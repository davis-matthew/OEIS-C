/*
 * A205983  Number of (n+1)X6 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A205983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 156 * a(n-3) for n > 10 Initial values a(1)=60432,
 *            a(2)=140802, a(3)=408411, a(4)=1814784, a(5)=9653376,
 *            a(6)=50843904, a(7)=279003648, a(8)=1502833632, a(9)=7930959552,
 *            a(10)=43524639072
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 21, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for 1 ≤ n ≤ 10 */
    static const seq_t base[10] = {
        60432ULL,          /* a(1) */
        140802ULL,         /* a(2) */
        408411ULL,         /* a(3) */
        1814784ULL,        /* a(4) */
        9653376ULL,        /* a(5) */
        50843904ULL,       /* a(6) */
        279003648ULL,      /* a(7) */
        1502833632ULL,     /* a(8) */
        7930959552ULL,     /* a(9) */
        43524639072ULL     /* a(10) */
    };

    /* Return base value directly for n ≤ 10 */
    if (n <= 10) {
        return base[n - 1];
    }

    /* Determine the appropriate base index (8, 9, or 10)
       that shares the same residue modulo 3 as n. */
    idx_t r = n % 3;
    idx_t base_n;                     /* actual index (1‑based) of the base value */
    seq_t res;                         /* current result */

    if (r == 0) {                    /* n ≡ 0 (mod 3) → use a(9) */
        base_n = 9;
        res    = base[8];
    } else if (r == 1) {              /* n ≡ 1 (mod 3) → use a(10) */
        base_n = 10;
        res    = base[9];
    } else {                          /* n ≡ 2 (mod 3) → use a(8) */
        base_n = 8;
        res    = base[7];
    }

    /* Multiply by 156 for every 3‑step back from base_n to n */
    idx_t steps = (n - base_n) / 3;
    for (idx_t i = 0; i < steps; ++i) {
        res *= (seq_t)156;
    }

    return res;
}
