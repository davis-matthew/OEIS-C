/*
 * A118433  Self-inverse triangle H, read by rows; a nontrivial matrix
 *          square-root of identity: H^2 = I, where H(n,k) =
 *          C(n,k)*(-1)^(floor((n+1)/2) - floor(k/2) + n - k) for n >= k >= 0.
 * https://oeis.org/A118433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the nth entry of the self‑inverse triangle H, read by
 *            rows. For row r and column k (0 ≤ k ≤ r), H(r,k) = binomial(r,k)
 *            * (-1)^( floor((r+1)/2) - floor(k/2) + r - k ) a(n) is obtained
 *            by determining the row r such that T(r-1) ≤ n < T(r) (with
 *            T(r-1) = r*(r+1)/2) and k = n - T(r-1), then co...
 * Range:     i8 n <= 38, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* My own integer square‑root for 64‑bit unsigned values.
   Returns floor(sqrt(a)). Works for all a that fit in uint64_t. */
static idx_t isqrt_u64(idx_t a)
{
    if (a == 0) return 0;
    /* Use double precision sqrt; the integer part is accurate for
       all values that fit in 64 bits (here a ≤ 8*n+1 with n ≤ 10000). */
    double r = sqrt((double)a);
    idx_t x = (idx_t)r;
    /* Correct possible off‑by‑one errors from the double approximation. */
    while ((unsigned __int128)(x + 1) * (x + 1) <= a) ++x;
    while ((unsigned __int128)x * x > a) --x;
    return x;
}

/* Multiplicative binomial coefficient: binomial(N,K) for integer N,K>=0.
   It uses only seq_t operations and divides exactly at each step. */
static seq_t binom_mult(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Main sequence function requested. */
seq_t a(idx_t n)
{
    /* r = floor( ( sqrt(1+8n) - 1 ) / 2 )  → row index */
    idx_t r = (isqrt_u64(1ULL + (unsigned __int128)8 * n) - 1) >> 1;

    /* offset = T(r-1) = r*(r+1)/2 */
    idx_t offset = (unsigned __int128)r * (r + 1) / 2;

    idx_t k = n - offset;                /* column index within row r */

    /* Compute parity of the exponent:
       ((r+1)/2) - (k/2) + r - k   (mod 2)  → XOR of the four terms. */
    uint8_t parity = (uint8_t)(
        ((r + 1) >> 1) ^ (k >> 1) ^ r ^ k
    ) & 1U;

    seq_t coeff = binom_mult(r, k);

    if (parity) coeff = -coeff;          /* apply the sign */

    return coeff;
}
