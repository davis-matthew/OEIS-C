/*
 * A074414  Number of 5-ary Lyndon words of length n over Z_5 with trace 0 and
 *          subtrace 0.
 * https://oeis.org/A074414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of Lyndon words of length n over Z_5 with trace =
 *            0 and subtrace = 0. Computed as a(n) = N_lex(n) – Σ_{d|n, d<n}
 *            a(d), where N_lex(n) = (1/n) * Σ_{d|n} φ(n/d) * H(d,n), H(d,n) =
 *            5^d if (n/d)%5==0, S[d] otherwise, S[d] = number of ordinary
 *            length‑d strings over Z_5 with trace = subtr...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   compute_lyndon_any, compute_powers, compute_strings,
 *            compute_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_lyndon_any.h"
#include "../utils/compute_powers.h"
#include "../utils/compute_strings.h"
#include "../utils/compute_totient.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    size_t N = (size_t)n + 1;
    seq_t *S   = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *pow5 = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *L_any= (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *a_val= (seq_t *)calloc(N, sizeof(seq_t));
    uint64_t *phi= (uint64_t *)calloc(N, sizeof(uint64_t));

    if (!S || !pow5 || !L_any || !a_val || !phi) {
        /* memory failure – release any allocated memory */
        free(S); free(pow5); free(L_any); free(a_val); free(phi);
        return 0;
    }

    compute_totient(n, phi);
    compute_powers(n, pow5);
    compute_strings(n, S);
    compute_lyndon_any(n, pow5, L_any);

    /* Temporary array to hold divisors of current length. */
    idx_t divs[128];
    idx_t dcnt;

    for (idx_t l = 1; l <= n; ++l) {
        /* Gather divisors of l. */
        dcnt = 0;
        for (idx_t d = 1; d * d <= l; ++d) {
            if (l % d == 0) {
                divs[dcnt++] = d;
                if (d != l / d) divs[dcnt++] = l / d;
            }
        }

        /* Burnside: sum over divisors. */
        seq_t sum_fixes = 0;
        for (idx_t i = 0; i < dcnt; ++i) {
            idx_t d = divs[i];
            idx_t f = l / d;                /* factor = l/d */
            seq_t H;
            if (f % 5 == 0) H = pow5[d];
            else            H = S[d];
            sum_fixes += (seq_t)phi[f] * H;
        }
        seq_t N_lex = sum_fixes / l;        /* necklaces with property */

        /* Subtract necklaces from proper divisors. */
        seq_t sum_sub = 0;
        for (idx_t i = 0; i < dcnt; ++i) {
            idx_t d = divs[i];
            if (d == l) continue;
            idx_t f = l / d;
            seq_t Y;
            if (f % 5 == 0) Y = L_any[d];
            else            Y = a_val[d];
            sum_sub += Y;
        }

        a_val[l] = N_lex - sum_sub;
    }

    seq_t result = a_val[n];

    free(S); free(pow5); free(L_any); free(a_val); free(phi);
    return result;
}
