/*
 * A082421  Numbers n such that P(n+1) < P(n) < P(n+2), where P(n) = largest
 *          prime factor of n (A006530).
 * https://oeis.org/A082421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer n such that * largest_prime_factor(n+1)
 *            < largest_prime_factor(n) < * largest_prime_factor(n+2). * * The
 *            function keeps a static cache of found terms and a static array
 *            * containing largest prime factors for all integers up to the
 *            current * maximum checked value. Whenever...
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *lpf = NULL;          /* largest prime factor array */
static size_t   lpf_cap = 0;          /* current size of lpf array */

static void
ensure_lpf_cap(size_t limit)
{
    if (limit <= lpf_cap) return;

    /* reallocate to new capacity */
    uint32_t *new_lpf = (uint32_t *)realloc(lpf, (limit + 1) * sizeof(uint32_t));
    if (!new_lpf) return;            /* allocation failure – give up silently */
    lpf = new_lpf;
    for (size_t i = 0; i <= limit; ++i) lpf[i] = 0;
    lpf_cap = limit;

    /* Simple sieve filling largest prime factor */
    for (size_t p = 2; p <= limit; ++p) {
        if (lpf[p] == 0) {           /* p is prime */
            for (size_t m = p; m <= limit; m += p)
                lpf[m] = (uint32_t)p;
        }
    }
}

/* Cache of sequence terms */
static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;
static size_t seq_len  = 0;
static size_t last_m_checked = 0;    /* last integer m that was examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for index 0 */

    /* initialise cache if first call */
    if (seq_cache == NULL) {
        seq_cap = 128;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len  = 0;
    }

    /* ensure we have enough lpf values for future scanning */
    /* We will grow the lpf array in steps of *2 whenever needed */
    if (lpf_cap == 0) ensure_lpf_cap(1024);

    /* generate terms until we have at least n stored */
    while ((idx_t)seq_len < n) {
        size_t m = ++last_m_checked;

        /* make sure lpf has entries up to m+2 */
        if (m + 2 > lpf_cap)
            ensure_lpf_cap((m + 2) * 2);

        uint32_t p1 = lpf[m + 1];
        uint32_t p  = lpf[m];
        uint32_t p2 = lpf[m + 2];

        if (p1 < p && p < p2) {
            /* add m to cache */
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 128;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)m;
        }
    }

    /* a(n) is the (n-1)-th stored value (0‑based) */
    return seq_cache[(size_t)(n - 1)];
}
