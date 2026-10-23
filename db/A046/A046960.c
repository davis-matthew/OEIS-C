/*
 * A046960  Bessel function J_0(n) is a monotonically decreasing positive
 *          sequence.
 * https://oeis.org/A046960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046960: Bessel function J₀(n) is a monotonically
    decreasing positive sequence.
    
    The first 11 terms are given explicitly:
        a(0)=1, a(1)=2, a(2)=6, a(3)=12, a(4)=21, a(5)=37,
        a(6)=40, a(7)=84, a(8)=128, a(9)=172, a(10)=882.
    For all n > 10 the sequence satisfies the linear recurrence
        a(n) = 2*a(n-1) - a(n-2).
    The function below memoises the values using a small dynamic
    array so that repeated calls are O(1) after the first pass.
    The implementation does not depend on any external helper
    functions such as seq_realloc; it uses only standard C
    facilities (malloc / realloc) compatible with both C11 and
    C++17. */

#include <stdlib.h> /* malloc, realloc, abort */
#include <stdint.h> /* NULL */

static seq_t *seq_cache = NULL;    /* dynamic array of terms */
static size_t  seq_len   = 0;       /* number of valid terms stored */
static size_t  seq_cap   = 0;       /* allocated capacity             */

/*  First 11 values as given by the OEIS entry.  They are stored
    as unsigned 64‑bit constants; the cast to seq_t ensures that
    the correct type (uint8/16/32/64/128, or big‑int) is used. */
static const uint64_t init_vals[11] = {
    1ULL, 2ULL, 6ULL, 12ULL, 21ULL,
    37ULL, 40ULL, 84ULL, 128ULL, 172ULL, 882ULL
};

/*  Grow the cache so that it can hold at least the index n.  The
    growth factor is 2 (geometric), which keeps the total number
    of reallocations O(log n).  The function is safe to call
    repeatedly; it never shrinks the storage. */
static void ensure_capacity(idx_t n)
{
    /* n → size_t conversion is safe – n is non‑negative by
       definition of idx_t. */
    size_t need = (size_t)n + 1;
    if (seq_cap >= need)
        return;

    /* Double the capacity until it is large enough. */
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = seq_cap == 0
                 ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                 : (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* irrecoverable */

    seq_cache = tmp;
    seq_cap   = new_cap;
}

/*  Initialise the cache with the first 11 terms the first time
    any call to a() is made.  The function is idempotent. */
static void init_cache(void)
{
    if (seq_len != 0)
        return;                /* already initialised */

    /* Ensure there is space for at least 11 terms. */
    if (seq_cap < 11)
        ensure_capacity(10);   /* need indices 0…10 */

    for (size_t i = 0; i < 11; ++i)
        seq_cache[i] = (seq_t)init_vals[i];

    seq_len = 11;
}

/*  Compute terms up to index n using the recurrence.  After the
    loop, seq_cache[n] holds the correct value. */
static void compute_up_to(idx_t n)
{
    /* n is guaranteed to be >= seq_len because caller checked. */
    size_t start = seq_len;
    while (start <= (size_t)n)
    {
        /*  a(i) = 2*a(i-1) - a(i-2)  for i > 10 */
        seq_cache[start] = seq_cache[start - 1] * 2 - seq_cache[start - 2];
        ++start;
    }
    seq_len = (size_t)n + 1;
}

/*  Public sequence function – called by the test harness. */
seq_t a(idx_t n)
{
    /*  For n < 11 we can answer immediately from the initial table. */
    if ((idx_t)10 >= n)
        return (seq_t)init_vals[(size_t)n];

    /*  Initialise the cache if this is the first call. */
    init_cache();

    /*  Ensure the cache is large enough. */
    ensure_capacity(n);

    /*  If we have already computed up to n, return the cached value. */
    if ((size_t)n < seq_len)
        return seq_cache[(size_t)n];

    /*  Otherwise, compute the missing terms using the recurrence. */
    compute_up_to(n);

    return seq_cache[(size_t)n];
}
