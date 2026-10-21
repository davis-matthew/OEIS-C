/*
 * A082910  a(n) = prime(prime(n+1)-prime(n)).
 * https://oeis.org/A082910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime( prime(n+1) – prime(n) ) (offset 1)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Prime storage as 64‑bit values – safe for all term sizes in this sequence. */
static uint64_t *prime_vals = NULL;
static idx_t prime_cnt = 0;

/* Build a sieve up to LIMIT and store all primes in prime_vals.  LIMIT must
   exceed the 10001st prime (~104 729) to support n up to 10 000. */
static void init_primes(void)
{
    if (prime_vals != NULL)
        return;

    const idx_t LIMIT = 2000000;                     /* ample margin */
    unsigned char *is_comp = (unsigned char *)malloc((size_t)(LIMIT + 1));
    memset(is_comp, 0, (size_t)(LIMIT + 1));

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_comp[i] == 0) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* Count primes. */
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_comp[i] == 0)
            ++prime_cnt;

    prime_vals = (uint64_t *)malloc((size_t)prime_cnt * sizeof(uint64_t));
    idx_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_comp[i] == 0)
            prime_vals[pos++] = (uint64_t)i;   /* store as 64‑bit */

    free(is_comp);
}

/* a(n) as defined in OEIS A082910.  The function is safe for any
   n >= 1, and does not rely on monotonic calls. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                      /* undefined below offset */

    init_primes();

    /* Ensure we have at least n+1 primes. */
    if ((idx_t)(n + 1) > prime_cnt)
        return (seq_t)0;                      /* safety check */

    /* Prime values at positions n   and n+1 (1‑based). */
    uint64_t p_n     = prime_vals[n - 1];
    uint64_t p_next  = prime_vals[n];

    /* Gap between them.  The prime gap is small (at most a few hundred
       for the range we support), so it fits in idx_t. */
    idx_t gap = (idx_t)(p_next - p_n);       /* unsigned difference */

    if (gap == 0 || (idx_t)gap > prime_cnt)
        return (seq_t)0;                      /* should never happen */

    /* Return the prime at index 'gap' (1‑based). */
    return (seq_t)prime_vals[gap - 1];
}
