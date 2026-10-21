/*
 * A201507  Number of ways to place 7 nonattacking wazirs on an n X n board.
 * https://oeis.org/A201507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n>=6, a(n) = (n^14 - 105*n^12 + 84*n^11 + 4795*n^10 -
 *            7560*n^9 - 119595*n^8 + 280980*n^7 + 1660204*n^6 - 5360880*n^5 -
 *            10985940*n^4 + 52150896*n^3 + 7858080*n^2 - 205168320*n +
 *            187629120) / 5040 For n<6: a(1)=0, a(2)=0, a(3)=0, a(4)=20,
 *            a(5)=12798
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset terms directly */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 0;
    if (n == 4) return 20;
    if (n == 5) return 12798;

    /* For n >= 6 use the explicit polynomial expression.
       Use a 128‑bit intermediate when seq_t is a 64‑bit integer
       to avoid overflow in the powers.  Otherwise rely on
       arbitrary‑precision seq_t arithmetic. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* 64‑bit or smaller: use __uint128_t for intermediate work */
        __uint128_t p[15];
        p[0] = 1;
        for (idx_t i = 1; i <= 14; ++i) {
            p[i] = p[i - 1] * (unsigned long long)n;
        }

        __uint128_t num = 0;
        num += p[14];
        num -= p[12] * (unsigned long long)105;
        num += p[11] * (unsigned long long)84;
        num += p[10] * (unsigned long long)4795;
        num -= p[ 9] * (unsigned long long)7560;
        num -= p[ 8] * (unsigned long long)119595;
        num += p[ 7] * (unsigned long long)280980;
        num += p[ 6] * (unsigned long long)1660204;
        num -= p[ 5] * (unsigned long long)5360880;
        num -= p[ 4] * (unsigned long long)10985940;
        num += p[ 3] * (unsigned long long)52150896;
        num += p[ 2] * (unsigned long long)7858080;
        num -= p[ 1] * (unsigned long long)205168320;
        num += (unsigned long long)187629120;

        return (seq_t)(num / (unsigned long long)5040);
    } else {
        /* seq_t is an arbitrary‑precision type (or >64 bits) */
        seq_t pow[15];
        pow[0] = 1;
        for (idx_t i = 1; i <= 14; ++i) {
            pow[i] = pow[i - 1] * (seq_t)n;
        }

        seq_t num = 0;
        num += pow[14];
        num -= pow[12] * (seq_t)105;
        num += pow[11] * (seq_t)84;
        num += pow[10] * (seq_t)4795;
        num -= pow[ 9] * (seq_t)7560;
        num -= pow[ 8] * (seq_t)119595;
        num += pow[ 7] * (seq_t)280980;
        num += pow[ 6] * (seq_t)1660204;
        num -= pow[ 5] * (seq_t)5360880;
        num -= pow[ 4] * (seq_t)10985940;
        num += pow[ 3] * (seq_t)52150896;
        num += pow[ 2] * (seq_t)7858080;
        num -= pow[ 1] * (seq_t)205168320;
        num += (seq_t)187629120;

        return num / (seq_t)5040;
    }
}
