/*
 * A194662  Number of k in [1,n] for which <n*r>+<k*r> > 1, where < > =
 *          fractional part and r = (1+sqrt(5))/2 (the golden ratio); row sums
 *          of A164661.
 * https://oeis.org/A194662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{ k ∈ [1,n] | {n*φ} + {k*φ} > 1 },
 * where φ = (1+√5)/2 and {x} is the fractional part of x.
 *
 * The sequence is computed by using the Beatty property
 * {k*φ} = {k*(φ-1)}.  The irrational number φ-1 = 1/φ
 * is approximated by the 22‑nd convergent of its continued
 * fraction:  φ-1 ≈ 10946/17711.  For n ≤ 100 000 this
 * approximation gives the exact integer floor for k ≤ n,
 * because the error is < 3·10⁻⁵ and the potential
 * mis‑flooring would require an error ≥ 0.5.
 *
 * Using this rational approximation we can test the
 * inequality
 *
 *   {k*φ} > 1 – {n*φ}  ⇔  r_k > D – r_n
 *
 * where r_k = (k * NUM) mod DEN with NUM=10946,
 * DEN=17711 and r_n = (n * NUM) mod DEN.
 * The count of such k is a(n).
 */

#include <stdint.h>
#include <stddef.h>

static const unsigned long long NUM = 10946ULL;   /* F21 */
static const unsigned long long DEN = 17711ULL;   /* F22 */

seq_t a(idx_t n)
{
    /* compute the remainder r_n = (n * NUM) % DEN   */
    __uint128_t prod_n = ( __uint128_t )n * NUM;
    unsigned long long r_n   = (unsigned long long)( prod_n % DEN );
    unsigned long long thresh = DEN - r_n;          /* D - r_n */

    seq_t count = 0;

    for ( idx_t k = 1; k <= n; ++k )
    {
        __uint128_t prod_k = ( __uint128_t )k * NUM;
        unsigned long long r_k = (unsigned long long)( prod_k % DEN );
        if ( r_k > thresh ) ++count;
    }

    return count;
}
