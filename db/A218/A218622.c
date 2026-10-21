/*
 * A218622  a(n) = A183161(n) (mod 4), n>=0.
 * https://oeis.org/A218622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2048, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   binom_get, binom_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A218622
    a(n) = A183161(n)  (mod 4),  n ≥ 0
    a(0) = 1
    Recurrence used:
        2 a(n) = b(n) – Σ_{k=1}^{n‑1} a(k) a(n–k)
    where
        b(n) = Σ_{k=0}^{n} C(n+k, n‑k) · C(2n–k, k)
    All arithmetic is performed modulo 8; the final division by 2
    yields a(n) modulo 4.  The binomial coefficients C(m,r) are
    computed modulo 8 using a Pascal triangle stored in 3‑bit
    packed form.  The triangle is built once for rows up to
    2·MAXN (MAXN = 10 000) and reused for subsequent calls.  The
    function keeps a cache of already computed a(n) values so that
    arbitrary order of calls is handled in O(max(n)) time and
    memory.  The implementation is fully compliant with C11 and
    C++17.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binom_get.h"
#include "../utils/binom_set.h"

/* ---------- 3‑bit packed Pascal triangle  ----------------- */

/* ----------  Main sequence function  ----------------------- */

seq_t a(idx_t n)
{
    /* Pre‑defined maximum n ------------------------------------------------ */
    static const idx_t MAXN = 10000;           /* largest n needed by the tests   */
    static const idx_t MAX_ROW = 2 * MAXN;     /* rows needed for binomials       */
    /* Static storage -------------------------------------------------------- */
    static uint32_t *binom_data = NULL;
    static seq_t   *a_cache    = NULL;
    static idx_t    cache_len  = 0;
    static int      built      = 0;            /* flag that indicates tables are ready */

    /* Build binomial table and cache once ----------------------------------- */
    if (!built)
    {
        /* Total entries in the upper triangle of size MAX_ROW+1 */
        size_t total_entries = ((size_t)(MAX_ROW + 1)) * ((size_t)(MAX_ROW + 2)) / 2;
        size_t words         = (total_entries + 9) / 10;   /* ceiling( /10 ) */

        binom_data = (uint32_t *)malloc(words * sizeof(uint32_t));
        if (!binom_data) exit(1);              /* allocation failed – abort */
        memset(binom_data, 0, words * sizeof(uint32_t));

        /* Build Pascal triangle modulo 8 ------------------------------------- */
        idx_t r;
        for (r = 0; r <= MAX_ROW; ++r)
        {
            binom_set(binom_data, r, 0, 1);
            if (r > 0) binom_set(binom_data, r, r, 1);
            for (idx_t k = 1; k < r; ++k)
            {
                uint8_t val = (binom_get(binom_data, r - 1, k - 1) +
                               binom_get(binom_data, r - 1, k)) & 0x7;
                binom_set(binom_data, r, k, val);
            }
        }

        /* Allocate cache for a(n) values ------------------------------------- */
        a_cache = (seq_t *)malloc((MAXN + 1) * sizeof(seq_t));
        if (!a_cache) exit(1);
        a_cache[0] = 1;
        cache_len = 1;
        built = 1;
    }

    /* Extend cache up to requested n --------------------------------------- */
    if (n >= cache_len)
    {
        idx_t m;
        for (m = cache_len; m <= n; ++m)
        {
            uint8_t b = 0;  /* b(m) modulo 8 */
            for (idx_t k = 0; k <= m; ++k)
            {
                uint8_t c1 = binom_get(binom_data, m + k, m - k);   /* C(m+k, m-k) */
                uint8_t c2 = binom_get(binom_data, 2 * m - k, k);   /* C(2m-k, k) */
                b += (uint8_t)((c1 * c2) & 0x7);
                b &= 0x7;
            }

            uint8_t c = 0;  /* Σ_{k=1}^{m‑1} a(k)a(m‑k) modulo 8 */
            for (idx_t k = 1; k < m; ++k)
            {
                uint8_t prod = ((uint8_t)((a_cache[m - k] * a_cache[k]) & 0x7));
                c += prod;
                c &= 0x7;
            }

            uint8_t diff = (b + 8 - c) & 0x7;       /* even: 0,2 or 4         */
            uint8_t a_val = (diff >> 1) & 0x3;      /* a(m) modulo 4          */
            a_cache[m] = a_val;
        }
        cache_len = n + 1;
    }

    return a_cache[n];
}
