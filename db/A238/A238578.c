/*
 * A238578  Expansion of -(-4*x^4 + sqrt(-4*x^2-4*x+1) * (2*x^3+x^2-2*x)
 *          -12*x^3-7*x^2+2*x) / (sqrt(-4*x^2-4*x+1) * (4*x^3+8*x^2+3*x-1) -
 *          4*x^3-8*x^2-3*x+1).
 * https://oeis.org/A238578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{i=0..(n-k)}
 *            C(k,n-k-i)*C(k+i-1,k-1)*C(n-1,k-1) Recurrence (for n > 5):
 *            (n-5)*(n-1)*a(n) = (3*n^2-20*n+23)*a(n-1) +
 *            2*(n-2)*(4*n-19)*a(n-2) + 4*(n-4)*(n-3)*a(n-3) Initial values:
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=11, a(4)=45, a(5)=191
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[6] = {0, 1, 3, 11, 45, 191};

seq_t a(idx_t n)
{
    if (n < 6)
        return init_vals[n];

    /* ring buffer holding a(n-5) … a(n) */
    seq_t buf[6];
    for (size_t k = 0; k < 6; ++k)
        buf[k] = init_vals[k];

    for (idx_t i = 6; i <= n; ++i)
    {
        unsigned long long coeff1 = 3ULL * i * i - 20ULL * i + 23ULL;
        unsigned long long coeff2 = 2ULL * (i - 2) * (4ULL * i - 19ULL);
        unsigned long long coeff3 = 4ULL * (i - 4) * (i - 3);
        unsigned long long denom_ll = (i - 5) * (i - 1);
        seq_t denom = (seq_t)denom_ll;

        size_t idx1 = (size_t)((i - 1) % 6);
        size_t idx2 = (size_t)((i - 2) % 6);
        size_t idx3 = (size_t)((i - 3) % 6);

        seq_t new_val;
        if (sizeof(seq_t) <= 8) {
            /* Use 128‑bit intermediates to avoid overflow when seq_t is 8 bytes or less */
            unsigned __int128 prod1 = (unsigned __int128)buf[idx1] * coeff1;
            unsigned __int128 prod2 = (unsigned __int128)buf[idx2] * coeff2;
            unsigned __int128 prod3 = (unsigned __int128)buf[idx3] * coeff3;
            unsigned __int128 numer = prod1 + prod2 + prod3;
            new_val = (seq_t)(numer / denom_ll);
        } else {
            /* seq_t is an arbitrary‑precision type; normal multiplication suffices */
            seq_t prod1 = buf[idx1] * (seq_t)coeff1;
            seq_t prod2 = buf[idx2] * (seq_t)coeff2;
            seq_t prod3 = buf[idx3] * (seq_t)coeff3;
            seq_t numer = prod1 + prod2 + prod3;
            new_val = numer / denom;
        }

        buf[(size_t)(i % 6)] = new_val;
    }

    return buf[(size_t)(n % 6)];
}
