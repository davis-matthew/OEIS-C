/*
 * A212594  a(n) is the difference between multiples of 11 with even and odd
 *          decimal digit sum in interval [0,10^n).
 * https://oeis.org/A212594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = N(n,0,0) – N(n,0,1) where N(k,r,p) is the number of
 *            k‑digit decimal strings (allowing leading zeros) whose value
 *            modulo 11 equals r and whose digit‑sum parity is p
 *            (0=even,1=odd). The function computes these counts by dynamic
 *            programming over digits
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* internal state for memoisation -------------------------------------------------- */
static seq_t dp_prev[22];
static seq_t dp_cur[22];
static idx_t cur_len = 0;               /* length of dp_prev (number of digits processed) */
static seq_t *a_cache = NULL;           /* cache of computed a(n) values   */
static idx_t cache_size = 0;            /* size of a_cache (n+1)           */
static int initialized = 0;

/* helper: compute next state indices ---------------------------------------------- */
static inline void update_dp(void)
{
    memset(dp_cur, 0, sizeof(dp_cur));

    for (idx_t i = 0; i < 22; ++i) {
        seq_t cnt = dp_prev[i];
        if (!cnt) continue;           /* nothing to propagate */

        unsigned mod   = i / 2;       /* current modulo 11   */
        unsigned parity = i % 2;      /* current parity     */

        for (unsigned d = 0; d < 10; ++d) {
            unsigned next_mod   = (mod * 10 + d) % 11;
            unsigned next_parity = parity ^ (d & 1);
            idx_t j = next_mod * 2 + next_parity;
            dp_cur[j] += cnt;
        }
    }
}

/* main sequence function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise static data on first call ------------------------------------ */
    if (!initialized) {
        memset(dp_prev, 0, sizeof(dp_prev));
        dp_prev[0] = 1;   /* number 0 (length 0) has mod 0 and even parity */
        cur_len = 0;
        a_cache = NULL;
        cache_size = 0;
        initialized = 1;
    }

    if (n == 0)            /* not used, but keep defined */
        return 0;

    /* extend cache to required n ----------------------------------------- */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – return 0 to signal error */
            return 0;
        }
        a_cache = new_cache;
        /* initialise new entries to zero */
        for (idx_t i = cache_size; i <= n; ++i)
            a_cache[i] = 0;
        cache_size = n + 1;
    }

    /* compute new values up to n if needed ----------------------------- */
    for (idx_t step = cur_len + 1; step <= n; ++step) {
        update_dp();          /* dp_cur now holds counts for length = step */

        /* store a(step) = even - odd possibilities for mod 0 */
        a_cache[step] = dp_cur[0] - dp_cur[1];

        /* prepare for next iteration */
        memcpy(dp_prev, dp_cur, sizeof(dp_prev));
        /* dp_cur will be overwritten next time */
    }

    cur_len = n;
    return a_cache[n];
}
