/*
 * A089612  a(n) = ((-1)^(n+1)*A002425(n)) modulo 5.
 * https://oeis.org/A089612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (n odd ? 1 : 2^(v2(n)+1) mod 5)
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based. Return 1 for all odd n; for even n, compute
       a(n) = 2^(v2(n)+1) mod 5, where v2(n) is the exponent of 2 in n. */
    if ((n & 1) == 1)      /* odd */
        return (seq_t)1;

    /* even n: count trailing zeros (exponent of 2) */
    idx_t e = 0;
    while ((n & 1) == 0) {
        e++;
        n >>= 1;
    }

    /* compute 2^(e+1) mod 5 using period 4 */
    idx_t exp = e + 1;
    seq_t res;
    switch (exp & 3) {
        case 0: res = 1; break;  /* 2^4 ≡ 1 (mod 5) */
        case 1: res = 2; break;  /* 2^1 ≡ 2 (mod 5) */
        case 2: res = 4; break;  /* 2^2 ≡ 4 (mod 5) */
        case 3: res = 3; break;  /* 2^3 ≡ 8 ≡ 3 (mod 5) */
    }
    return res;
}
