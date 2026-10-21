/*
 * A235975  Number of (n+1)X(2+1) 0..2 arrays with the upper median equal to
 *          the lower median in every 2X2 subblock.
 * https://oeis.org/A235975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A235975 – Number of (n+1)×(2+1) 0..2 arrays with the
   upper median equal to the lower median in every 2×2 subblock.
   Recurrence (order 9):
   a(n) = 11*a(n-1) – 9*a(n-2) – 173*a(n-3) + 380*a(n-4) + 324*a(n-5)
          – 1056*a(n-6) + 176*a(n-7) + 256*a(n-8) – 64*a(n-9)
   Offset = 1 (a(1) = 181).  Base values for n=1…9 are listed in
   the source.
*/
#include <stddef.h>
#include <stdlib.h>

/* cached values: 0 … cache_max-1 correspond to a(1)…a(cache_max) */
static seq_t *cache      = NULL; /* array of seq_t */
static size_t cache_cap  = 0;    /* allocated capacity */
static idx_t  cache_max  = 0;    /* highest n already computed */

static const seq_t base_values[9] = {
    (seq_t)181,
    (seq_t)1473,
    (seq_t)11239,
    (seq_t)89555,
    (seq_t)703565,
    (seq_t)5579529,
    (seq_t)44112111,
    (seq_t)349456139,
    (seq_t)2766556005
};

static void ensure_capacity(idx_t need)
{
    if (cache_cap >= (size_t)need) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 32;
    while (new_cap < (size_t)need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* out of spec – return 0 safely */

    /* Initialise cache with the first nine terms on first use */
    if (cache_max == 0) {
        ensure_capacity(9);
        for (idx_t i = 0; i < 9; ++i) cache[i] = base_values[i];
        cache_max = 9;
    }

    if (n <= cache_max) return cache[n-1];

    ensure_capacity(n);

    /* coefficients for the recurrence (when n > 9) */
    static const int coeff[9] = {
        11,  -9,  -173, 380, 324, -1056, 176, 256, -64
    };

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 9) {
            val = base_values[k-1];
        } else {
            seq_t sum_pos = 0, sum_neg = 0;
            for (int i = 0; i < 9; ++i) {
                idx_t idx   = k - i - 1;          /* a(k-i-1) */
                seq_t term  = cache[idx-1];
                int   c     = coeff[i];
                if (c >= 0)
                    sum_pos += term * (seq_t)c;
                else
                    sum_neg += term * (seq_t)(-c);
            }
            val = sum_pos - sum_neg; /* subtraction preserves positivity */
        }
        cache[k-1] = val;
    }

    cache_max = n;
    return cache[n-1];
}
