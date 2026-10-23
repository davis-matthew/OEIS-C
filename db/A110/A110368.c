/*
 * A110368  Integers with mutual residues of 9.
 * https://oeis.org/A110368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ m > a(n-1) | m ≡ k (mod a(i)) for 1 ≤ i < n }, k = 9
 *            where a(1) = k + 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* ---------- Helper: Euclidean gcd for seq_t ---------- */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *vals      = NULL;   /* a[0] = a(1), ... */
    static seq_t *lcm_arr   = NULL;   /* lcm of previous terms */
    static size_t capacity  = 0;      /* allocated capacity */
    static idx_t  computed  = 0;      /* number of terms stored */

    if (n == 0) return 0;                 /* undefined, safe fallback */

    /* Resize arrays if necessary */
    if (n > computed) {
        size_t need = (size_t)n;
        if (capacity < need) {
            size_t new_cap = capacity ? capacity << 1 : 8;
            while (new_cap < need) new_cap <<= 1;

            seq_t *new_vals  = (seq_t *)malloc(new_cap * sizeof(seq_t));
            seq_t *new_lcm   = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_vals || !new_lcm) { /* allocation failed */
                free(new_vals);
                free(new_lcm);
                return 0;
            }

            if (computed > 0) {
                memcpy(new_vals, vals,
                       computed * sizeof(seq_t));
                memcpy(new_lcm, lcm_arr,
                       computed * sizeof(seq_t));
                free(vals);
                free(lcm_arr);
            }
            vals      = new_vals;
            lcm_arr   = new_lcm;
            capacity  = new_cap;
        }

        const seq_t k = 9;   /* residue */

        /* Compute missing terms up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {                /* base case */
                vals[0] = k + 1;
                lcm_arr[0] = vals[0];
            } else {
                seq_t prev_val = vals[i - 2];
                seq_t L_prev = lcm_arr[i - 2];

                /* t = floor((prev_val - k)/L_prev) + 1 */
                seq_t t = ((prev_val - k) / L_prev) + 1;

                /* new value */
                seq_t new_val = k + L_prev * t;
                vals[i - 1] = new_val;

                /* update lcm: L_new = lcm(L_prev, new_val) */
                seq_t g = gcd_seq_t(L_prev, new_val);
                seq_t L_new = (L_prev / g) * new_val;
                lcm_arr[i - 1] = L_new;
            }
        }
        computed = n;
    }

    return vals[n - 1];
}
