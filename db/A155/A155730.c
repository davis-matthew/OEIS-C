/*
 * A155730  Indices of Bell numbers that are divisible by 5.
 * https://oeis.org/A155730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indices of Bell numbers that are divisible by 5.
   The differences of successive indices repeat with period 156:
   a(n) = a(1) + ⌊(n-1)/156⌋·Σ + pref[ (n-1)%156 ]  where
   Σ is the sum of the 156 differences and pref[k] = Σ_{i=0}^k diff[i].
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment */
seq_t a(idx_t n)
{
    /* static state for the first 157 indices and periodic data */
    static bool initialized = false;
    static seq_t a_vals[158];          /* a_vals[1..157] */
    static uint8_t diff[156];          /* differences for one period */
    static seq_t prefix[156];          /* cumulative sums of diff */
    static seq_t period_sum;           /* total sum over one period */

    if (!initialized) {
        /* ------------------------------------------------------------------
           Compute the first 157 indices i such that Bell(i) ≡ 0 (mod 5).
         ------------------------------------------------------------------ */
        const idx_t TARGET = 157;
        idx_t zeros_found = 0;

        /* dynamic arrays for Bell values (mod 5) and binomial row (mod 5) */
        idx_t bal_cap = 64, bal_len = 1;
        unsigned char *bell = (unsigned char *)malloc(bal_cap * sizeof(unsigned char));
        bell[0] = 1;                       /* Bell(0) = 1 */

        idx_t bin_cap = 2, bin_len = 1;
        unsigned char *bin = (unsigned char *)malloc(bin_cap * sizeof(unsigned char));
        bin[0] = 1;                         /* C(0,0) = 1 */

        idx_t n = 0;                       /* current Bell index being processed */

        /* Loop until we have found 157 zeros */
        while (zeros_found < TARGET) {
            /* Compute Bell(n+1) = Σ_{k=0}^{n} C(n,k) * Bell(k) (mod 5) */
            unsigned char val = 0;
            for (idx_t k = 0; k < bin_len; ++k) {
                val += (unsigned char)((bin[k] * bell[k]) % 5);
                if (val >= 5) val -= 5;
            }
            val %= 5;

            /* Store the new Bell value */
            if (bal_len == bal_cap) {
                bal_cap <<= 1;
                bell = (unsigned char *)realloc(bell, bal_cap * sizeof(unsigned char));
            }
            bell[bal_len++] = val;

            /* Record index if Bell value is 0 mod 5 */
            if (val == 0) {
                ++zeros_found;
                a_vals[zeros_found] = (seq_t)(n + 1);   /* indices are 1‑based */
            }

            /* Build next binomial row for n+1 */
            idx_t new_bin_len = bin_len + 1;
            if (new_bin_len > bin_cap) {
                bin_cap = new_bin_len;
                bin = (unsigned char *)realloc(bin, bin_cap * sizeof(unsigned char));
            }
            unsigned char *next_bin = (unsigned char *)malloc(new_bin_len * sizeof(unsigned char));
            next_bin[0] = 1;
            next_bin[new_bin_len - 1] = 1;
            for (idx_t i = 1; i < new_bin_len - 1; ++i) {
                unsigned char v = 0;
                if (i - 1 < bin_len) v += bin[i - 1];
                if (i < bin_len)     v += bin[i];
                if (v >= 5) v -= 5;
                next_bin[i] = v;
            }
            free(bin);
            bin = next_bin;
            bin_len = new_bin_len;

            ++n;
        }

        free(bell);
        free(bin);

        /* ------------------------------------------------------------------
           Build difference array for the periodic part and its prefix sums.
         ------------------------------------------------------------------ */
        for (idx_t i = 0; i < 156; ++i) {
            seq_t d = a_vals[i + 2] - a_vals[i + 1];
            diff[i] = (uint8_t)d;
        }
        prefix[0] = diff[0];
        for (idx_t i = 1; i < 156; ++i) {
            prefix[i] = prefix[i - 1] + diff[i];
        }
        period_sum = prefix[155];

        initialized = true;
    }

    /* ----------------------------------------------------------------------
       Return the n‑th term using the periodic difference array.
     ---------------------------------------------------------------------- */
    if (n <= 157) {
        return a_vals[n];
    }

    idx_t diff_count = n - 1;           /* number of differences up to n */
    idx_t periods    = diff_count / 156;
    idx_t rem        = diff_count % 156;

    seq_t result = a_vals[1];
    if (periods > 0) {
        result += period_sum * (seq_t)periods;
    }
    if (rem > 0) {
        result += prefix[rem - 1];
    }
    return result;
}
