/*
 * A028699  Expansion of
 *          (theta_3(z)*theta_3(2z)*theta_3(4z)+theta_2(z)*theta_2(2z)*theta_2(4z))^2.
 * https://oeis.org/A028699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^n in
 *            (θ3(z)θ3(2z)θ3(4z)+θ2(z)θ2(2z)θ2(4z))^2, with t = q^{1/4}. This
 *            expands to integer‑indexed coefficients that match the OEIS
 *            sequence A028699
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   alloc_seq_array, idx2seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below precomputes the required coefficients up to a fixed
   maximum index (10000) and returns a(n) for any n in that range.  It uses
   only operations allowed by the specification and is fully
   deterministic and memoised. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* Provides seq_t *alloc_seq_array(size_t). */
#include "../utils/idx2seq.h"

#define MAXN 10000

static bool init_done = false;
static seq_t *A_t = NULL;
static seq_t *B_t = NULL;
static seq_t *S_t = NULL;
static seq_t *Res  = NULL;

static void init_sequence(void) {
    size_t N = MAXN;
    A_t = alloc_seq_array(N + 1);
    B_t = alloc_seq_array(N + 1);
    S_t = alloc_seq_array(N + 1);
    Res  = alloc_seq_array(N + 1);

    /* Initialise arrays to zero. */
    for (size_t i = 0; i <= N; ++i) {
        A_t[i] = 0;
        B_t[i] = 0;
        S_t[i] = 0;
        Res[i]  = 0;
    }

    /* Compute A_t: coefficients of θ3(z)θ3(2z)θ3(4z). */
    for (uint64_t a = 0; 4ULL*a*a <= N; ++a) {
        seq_t ca = (a == 0) ? 1 : 2;          /* coefficient from θ3(z) */
        uint64_t va = 4ULL * a * a;
        for (uint64_t b = 0; va + 8ULL*b*b <= N; ++b) {
            seq_t cb = (b == 0) ? 1 : 2;      /* coefficient from θ3(2z) */
            uint64_t vb = 8ULL * b * b;
            for (uint64_t c = 0; va + vb + 16ULL*c*c <= N; ++c) {
                seq_t cc = (c == 0) ? 1 : 2;  /* coefficient from θ3(4z) */
                uint64_t s = va + vb + 16ULL * c * c;
                seq_t w = ca * cb * cc;      /* weight of this triple */
                A_t[s] += w;
            }
        }
    }

    /* Compute B_t: coefficients of θ2(z)θ2(2z)θ2(4z). */
    for (uint64_t a = 0; 4ULL*a*a + 4ULL*a + 1ULL <= N; ++a) {
        uint64_t va = 4ULL * a * a + 4ULL * a + 1ULL;   /* exponent part from a */
        for (uint64_t b = 0; va + 8ULL*b*b + 8ULL*b + 2ULL <= N; ++b) {
            uint64_t vb = 8ULL * b * b + 8ULL * b + 2ULL;
            for (uint64_t c = 0; va + vb + 16ULL*c*c + 16ULL*c + 4ULL <= N; ++c) {
                uint64_t l = va + vb + 16ULL * c * c + 16ULL * c + 4ULL;
                /* Each θ2 term contributes a factor 2, so weight = 2^3 = 8. */
                B_t[l] += (seq_t)8;
            }
        }
    }

    /* Sum the two series. */
    for (size_t i = 0; i <= N; ++i) {
        S_t[i] = A_t[i] + B_t[i];
    }

    /* Convolution of S_t with itself to obtain the required coefficients. */
    for (size_t n = 0; n <= N; ++n) {
        seq_t val = 0;
        for (size_t i = 0; i <= n; ++i) {
            val += S_t[i] * S_t[n - i];
        }
        Res[n] = val;
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done) init_sequence();
    if (n > MAXN) return 0;           /* out‑of‑range request – return 0 */
    return Res[(size_t)n];
}
