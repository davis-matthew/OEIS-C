/*
 * A095140  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 5.
 * https://oeis.org/A095140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(i, j) mod 5, where i is the row such that
 *            i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 and j = n - i*(i+1)/2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   mul5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation assumes the existence of the types
 *  seq_t and idx_t.  It uses only integer arithmetic.  */
#include <stdint.h>
#include "../utils/mul5.h"

/*  Pre‑computed binomial coefficients modulo 5 for 0 ≤ n < 5. */
static const seq_t binom5[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,2,1,0,0},
    {1,3,3,1,0},
    {1,4,1,4,1}
};

/* Compute binomial(n, k) mod 5 using Lucas' theorem. */
static seq_t binom_mod5(idx_t n, idx_t k)
{
    seq_t res = 1;
    while (n || k) {
        idx_t ni = n % 5;
        idx_t ki = k % 5;
        if (ki > ni) return 0;
        res = mul5(res, binom5[ni][ki]);
        n /= 5;
        k /= 5;
    }
    return res;
}

/* Find the row index i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. */
static idx_t row_for_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Doubling search for an upper bound. */
    while ( ((unsigned __int128)high * (high + 1) / 2) <= n ) {
        high <<= 1;                 /* multiply by 2 */
        if (high > (UINT64_MAX >> 1)) { /* avoid overflow */
            high = UINT64_MAX >> 1;
            break;
        }
    }

    low = high >> 1;                /* it's our previous high */

    /* Binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 test = (unsigned __int128)mid * (mid + 1) / 2;
        if (test > n)
            high = mid;
        else
            low = mid + 1;
    }
    return low - 1;                  /* final row i */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Determine row i and column j. */
    idx_t i = row_for_index(n);
    unsigned __int128 offset = (unsigned __int128)i * (i + 1) / 2;
    idx_t j = n - (idx_t)offset;

    /* Compute binomial(i, j) mod 5. */
    return binom_mod5(i, j);
}
