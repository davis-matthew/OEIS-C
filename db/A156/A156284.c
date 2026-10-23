/*
 * A156284  From every interval (2^(m-1), 2^m), m >= 3, we remove primes p for
 *          which 2^m-p is a prime that was not removed for smaller values of
 *          m; the sequence gives all remaining odd primes.
 * https://oeis.org/A156284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A156284 - odd primes surviving the “remove p if 2^m‑p is an
    untouched prime” rule.
    The function builds the sequence incrementally: for each
    interval (2^(m‑1), 2^m) it removes primes p when
    (2^m – p) is prime and was not removed earlier.
    Surviving primes from the interval are appended to the
    sequence.  The first term (3) is added manually.
    The routine is memoised; any order of calls is supported.  */

#include <stdlib.h>
#include <stdbool.h>
#if defined(__cplusplus)
# include <new>
#endif

/* Static state ---------------------------------------------------------- */
static bool initialized   = false;
static idx_t current_m    = 2;      /* intervals processed up to m‑1   */
static idx_t current_limit = 0;     /* current sieve limit (max number sieved) */

/* Sieve and removal flags --------------------------------------------- */
static char *is_prime = NULL;      /* 1 = prime, 0 = composite   */
static char *removed  = NULL;      /* 1 = removed               */

/* Sequence storage ----------------------------------------------------- */
static seq_t *seq       = NULL;     /* sequence array (0‑based)   */
static idx_t seq_size   = 0;        /* number of terms stored    */
static size_t seq_cap   = 0;        /* allocated capacity        */

/* ----------------------------------------------------------------------- */
/* Allocate or reallocate the sequence array for at least 'new_cap' elems.  */
static void seq_ensure_cap(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;

    size_t old_cap = seq_cap;
    seq_cap   = new_cap;
    seq_t *new_seq = NULL;

#if defined(__cplusplus)
    /* allocate raw storage */
    new_seq = (seq_t *)::operator new(seq_cap * sizeof(seq_t));
    /* default‑construct all new elements */
    for (size_t i = 0; i < seq_cap; ++i)
        new (new_seq + i) seq_t();
    /* copy existing elements */
    for (size_t i = 0; i < seq_size; ++i)
        new_seq[i] = seq[i];
    /* destroy old objects */
    if (seq) {
        for (size_t i = 0; i < seq_size; ++i)
            seq[i].~seq_t();
        ::operator delete(seq);
    }
#else
    /* C: use malloc, no constructors/destructors needed */
    new_seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (new_seq && seq) {
        for (size_t i = 0; i < seq_size; ++i)
            new_seq[i] = seq[i];
        free(seq);
    }
#endif
    seq = new_seq;
}

/* Append a value to the sequence --------------------------------------- */
static void seq_push(seq_t val)
{
    if (seq_size == seq_cap)
        seq_ensure_cap(seq_cap == 0 ? 64 : seq_cap * 2);
    seq[seq_size++] = val;
}

/* Simple sieve of Eratosthenes up to 'limit' --------------------------- */
static void do_sieve(idx_t limit)
{
    /* allocate / resize is_prime array */
    if (!is_prime)
        is_prime = (char *)malloc((limit + 1) * sizeof(char));
    else
        is_prime = (char *)realloc(is_prime, (limit + 1) * sizeof(char));

    /* initialise all entries to prime */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
}

/* Extend data structures to handle next interval (2^(m‑1), 2^m) --------- */
static void process_next_interval(void)
{
    ++current_m;
    idx_t new_limit = ((idx_t)1) << current_m;   /* 2^m */

    /* ensure sieve up to new_limit */
    if (new_limit > current_limit) {
        idx_t old_limit = current_limit;
        current_limit  = new_limit;
        do_sieve(current_limit);

        /* extend removed array; zero new entries */
        if (!removed) {
            removed = (char *)calloc(current_limit + 1, sizeof(char));
        } else {
            removed = (char *)realloc(removed, (current_limit + 1) * sizeof(char));
            if (current_limit > old_limit) {
                for (idx_t i = old_limit + 1; i <= current_limit; ++i)
                    removed[i] = 0;
            }
        }
    }

    idx_t start = ((idx_t)1) << (current_m - 1); /* 2^(m‑1) */
    idx_t end   = current_limit;                 /* 2^m       */
    idx_t pow2m = end;

    /* Removal step */
    for (idx_t p = start + 1; p < end; ++p) {
        if (!is_prime[p]) continue;
        if (removed[p])   continue;
        idx_t q = pow2m - p;
        if (q <= current_limit && is_prime[q] && !removed[q])
            removed[p] = 1;          /* mark p as removed */
    }

    /* Append surviving primes */
    for (idx_t p = start + 1; p < end; ++p)
        if (is_prime[p] && !removed[p])
            seq_push((seq_t)p);
}

/* ----------------------------------------------------------------------- */
/* Public function required by the test harness --------------------------- */
seq_t a(idx_t n)
{
    /* One‑time initialisation: add the first term (3) */
    if (!initialized) {
        initialized = true;
        seq_push((seq_t)3);          /* a(1) = 3 */
    }

    /* Generate terms until we have enough */
    while (seq_size < n)
        process_next_interval();

    return seq[n - 1];
}
