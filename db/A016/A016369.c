/*
 * A016369  266th cyclotomic polynomial.
 * https://oeis.org/A016369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 266th cyclotomic polynomial
 *            Φ_266(x)
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t phi_266[267];
static idx_t deg_266;
static bool initialized = false;

/* Compute all cyclotomic polynomials φ_d for d dividing 266, then
   store φ_266 into the global array phi_266.  */
static void compute_phi266(void)
{
    const idx_t divisors[] = {1, 2, 7, 14, 19, 38, 133, 266};
    const int divcnt = (int)(sizeof(divisors) / sizeof(divisors[0]));

    seq_t local_phi[8][267];
    idx_t local_deg[8] = {0};

    for (int i = 0; i < divcnt; ++i) {
        idx_t d = divisors[i];

        /* Build denominator = product of φ_e for e | d and e < d */
        seq_t denom[267] = {0};
        denom[0] = (seq_t)1;               /* multiplicative identity */
        idx_t denom_deg = 0;

        for (int j = 0; j < i; ++j) {
            if (d % divisors[j] == 0) {
                /* multiply denom by φ_{divisors[j]} */
                seq_t tmp[267] = {0};
                idx_t new_deg = denom_deg + local_deg[j];
                for (idx_t a = 0; a <= denom_deg; ++a)
                    for (idx_t b = 0; b <= local_deg[j]; ++b)
                        tmp[a + b] += denom[a] * local_phi[j][b];
                for (idx_t k = 0; k <= new_deg; ++k)
                    denom[k] = tmp[k];
                denom_deg = new_deg;
            }
        }

        /* Numerator: x^d – 1 */
        seq_t num[267] = {0};
        num[0] = (seq_t)-1;
        num[d] = (seq_t)1;
        idx_t num_deg = d;

        /* Long division: num / denom = φ_d  */
        seq_t rem[267];
        idx_t rem_deg = num_deg;
        for (idx_t k = 0; k <= num_deg; ++k)
            rem[k] = num[k];

        idx_t res_deg = num_deg - denom_deg;
        seq_t res[267] = {0};

        for (int s = (int)res_deg; s >= 0; --s) {
            seq_t factor = rem[rem_deg];
            res[s] = factor;
            for (idx_t i = 0; i <= denom_deg; ++i)
                rem[s + i] -= factor * denom[i];
            --rem_deg;
        }

        for (idx_t k = 0; k <= res_deg; ++k)
            local_phi[i][k] = res[k];
        local_deg[i] = res_deg;
    }

    /* Store φ_266 */
    deg_266 = local_deg[divcnt - 1];
    for (idx_t k = 0; k <= deg_266; ++k)
        phi_266[k] = local_phi[divcnt - 1][k];
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_phi266();
        initialized = true;
    }
    if (n <= (idx_t)deg_266)
        return phi_266[(size_t)n];
    return (seq_t)0;
}
