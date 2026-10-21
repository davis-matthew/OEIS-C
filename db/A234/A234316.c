/*
 * A234316  Irregular triangle T, read by rows, such that row n lists the
 *          larger parts of the Goldbach partitions of 2n (in decreasing
 *          order).
 * https://oeis.org/A234316
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A234316:  a(n) = larger part of a Goldbach partition of 2n,
   read by rows (row n contains the larger primes in decreasing order). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* static state for caching sequence values */
static seq_t *seq_cache      = NULL;   /* array of computed values      */
static size_t seq_cap        = 0;      /* capacity of seq_cache          */
static size_t seq_len        = 0;      /* number of values stored        */
static idx_t next_row        = 2;      /* next row number to generate    */

/* static state for prime table */
static char   *prime_flag    = NULL;   /* 0/1 array: prime_flag[i] = 1 if i is prime */
static idx_t   prime_limit   = 0;      /* current maximum index of prime_flag */
static idx_t *prime_list     = NULL;   /* sorted list of primes            */
static size_t  prime_cnt     = 0;      /* number of primes in prime_list   */

/* Ensure that primes up to `limit` are available */
static void
ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free old data */
    free(prime_flag);
    free(prime_list);

    /* allocate and compute sieve */
    prime_flag = (char*)calloc((size_t)limit + 1, sizeof(char));
    if (!prime_flag)
        abort();
    /* mark all as prime initially (indices >= 2) */
    for (idx_t i = 2; i <= limit; ++i)
        prime_flag[i] = 1;

    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i)
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = 0;
        }

    /* build prime list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i])
            ++cnt;

    prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!prime_list)
        abort();

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i])
            prime_list[pos++] = i;

    prime_cnt   = cnt;
    prime_limit = limit;
}

/* Ensure cache capacity for at least `need` entries */
static void
ensure_seq_cache_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Append a value to the cached sequence */
static void
append_seq_value(seq_t val)
{
    ensure_seq_cache_cap(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;    /* offset behavior is irrelevant */

    size_t target = (size_t)(n - 2);   /* index within seq_cache */
    if (target < seq_len)
        return seq_cache[target];

    /* Generate rows until we reach the target entry */
    while (seq_len <= target) {
        idx_t r = next_row;               /* current row number */
        idx_t limit = r * 2;              /* need primes up to 2r */
        ensure_prime_limit(limit);

        /* For each prime p <= r, check whether q=2r-p is prime */
        for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= r; ++pi) {
            idx_t p = prime_list[pi];
            idx_t q = limit - p;           /* 2r - p */
            if (q <= prime_limit && prime_flag[q]) {
                append_seq_value((seq_t)q);
            }
        }
        ++next_row;
    }

    return seq_cache[target];
}
