/*
 * A095141  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 6.
 * https://oeis.org/A095141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(i,j) mod 6, where n = i*(i+1)/2 + j, 0 ≤ j ≤ i
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A095141: the triangle read by rows (Pascal's triangle mod 6). */
#include <stdint.h>

static const unsigned char small_bc[3][3] = {
    {1, 1, 1},   /* binom(0,0)=1 , binom(0,1)=0, binom(0,2)=0 (irrelevant) */
    {1, 1, 1},   /* binom(1,0)=1 , binom(1,1)=1 */
    {1, 2, 1}    /* binom(2,0)=1 , binom(2,1)=2 , binom(2,2)=1 */
};

seq_t a(idx_t n)
{
    /* Find row i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 */
    idx_t i = 0;
    while (1) {
        __int128 next_offset = (__int128)(i + 1) * (__int128)(i + 2) / 2;
        if (next_offset > n)
            break;
        ++i;
    }
    /* Compute column j */
    __int128 row_offset128 = (__int128)i * (__int128)(i + 1) / 2;
    idx_t row_offset = (idx_t)row_offset128;
    idx_t j = n - row_offset;

    /* 1. binomial(i,j) mod 2 */
    int mod2 = ((j & (i - j)) == 0) ? 1 : 0;   /* odd ↔ 1, even ↔ 0 */

    /* 2. binomial(i,j) mod 3 via Lucas theorem in base 3 */
    int mod3 = 1;
    uint64_t ii = i;
    uint64_t jj = j;
    while (ii > 0 || jj > 0) {
        unsigned ni = ii % 3;
        unsigned kj = jj % 3;
        if (kj > ni) {          /* digit of j exceeds digit of i → binomial 0 mod 3 */
            mod3 = 0;
            break;
        }
        mod3 = (mod3 * small_bc[ni][kj]) % 3;
        ii /= 3;
        jj /= 3;
    }

    /* 3. Combine residues to residue modulo 6 (CRT with 2 and 3) */
    uint64_t res = mod3;          /* res ≡ mod3 (mod 3) and 0 ≤ res < 3 */
    if ((res & 1) != (uint64_t)mod2)   /* adjust parity if necessary */
        res += 3;                 /* adding 3 flips parity, keeping result < 6 */
    return (seq_t)res;
}
