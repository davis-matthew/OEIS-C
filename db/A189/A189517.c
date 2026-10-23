/*
 * A189517  a(n) = n + [nr/t] + [ns/t]; r=1, s=Pi/2, t=2/Pi.
 * https://oeis.org/A189517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·π/2⌋ + ⌊n·π²/4⌋ (r = 1, s = π/2, t = 2/π) The
 *            function evaluates ⌊n·π/2⌋ and ⌊n·π²/4⌋ using fixed‑point
 *            arithmetic with a 53‑bit fractional precision. It keeps a static
 *            cache so that any call pattern is handled in O(max n) time. Only
 *            integer arithmetic is used; no floating‑point ope...
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* a(1)…a(max)        */
static size_t cap = 0;               /* array capacity     */
static idx_t computed = 0;           /* highest n computed */

static const unsigned __int128 SCALE = ((unsigned __int128)1) << 53;
static unsigned __int128 alpha_scaled; /* fractional part of π/2 scaled by SCALE */
static unsigned __int128 beta_scaled;  /* fractional part of π²/4 scaled by SCALE */

static unsigned __int128 frac_a = 0;   /* running sum for ⌊k·frac(π/2)⌋   */
static unsigned __int128 frac_b = 0;   /* running sum for ⌊k·frac(π²/4)⌋  */
static seq_t cur_a_frac = 0;           /* ⌊k·frac(π/2)⌋                   */
static seq_t cur_b_frac = 0;           /* ⌊k·frac(π²/4)⌋                  */

static void init_constants(void)
{
    const long double PI = 3.141592653589793238462643383279502884L;
    const long double alpha = PI / 2.0L;          /* π/2  ≈ 1.5707963267948966 */
    const long double beta  = (PI * PI) / 4.0L;   /* π²/4 ≈ 2.4674011002723397 */

    const long double alpha_frac = alpha - 1.0L;  /* 0.570796...   */
    const long double beta_frac  = beta  - 2.0L;  /* 0.467401...   */

    /* scale to integer (nearest) */
    alpha_scaled = (unsigned __int128)(alpha_frac * (long double)SCALE + 0.5L);
    beta_scaled  = (unsigned __int128)(beta_frac  * (long double)SCALE + 0.5L);
}

static void ensure_cap(size_t n)
{
    if (n <= cap) return;
    size_t new_cap = (cap == 0) ? 16 : cap * 2;
    while (new_cap < n) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

static void compute_upto(idx_t n)
{
    if (n <= computed) return;
    if (cap == 0) init_constants();

    ensure_cap(n);
    for (idx_t k = computed + 1; k <= n; ++k)
    {
        /* floor(k·π/2) : split integer part 1 * k + fractional part   */
        frac_a += alpha_scaled;
        if (frac_a >= SCALE) { frac_a -= SCALE; cur_a_frac += 1; }

        /* floor(k·π²/4) : integer part 2 * k + fractional part          */
        frac_b += beta_scaled;
        if (frac_b >= SCALE) { frac_b -= SCALE; cur_b_frac += 1; }

        /* a(k) = 4*k + floor(k·frac(π/2)) + floor(k·frac(π²/4))        */
        cache[k-1] = (seq_t)4 * k + cur_a_frac + cur_b_frac;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* below offset: undefined          */
    compute_upto(n);
    return cache[n-1];
}
