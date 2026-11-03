/*
 * A000993  Number of distinct quadratic residues mod 10^n; also number of
 *          distinct n-digit endings of base-10 squares.
 * https://oeis.org/A000993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = s2(n)*s5(n), where s_p(n) = number of squares mod p^n = 1
 *            + Sum_{m = n, n-2, ..., m >= 1} U_p(m), with U_p(m) the number
 *            of unit squares mod p^m: U_5(m) = 2*5^(m-1); U_2(1) = U_2(2) =
 *            1, U_2(m) = 2^(m-3) for m >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 807 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a000993_count2(idx_t n) {
    seq_t s = 1;
    idx_t m;
    seq_t pw;
    if (n == 0) return s;
    if (n % 2 == 1) {
        /* m = 1: 1; m = 3: 1; m = 5: 4; ... */
        s += 1;
        pw = 1;
        for (m = 3; m <= n; m += 2) {
            s += pw;
            if (m + 2 <= n) pw *= 4;
        }
    } else {
        /* m = 2: 1; m = 4: 2; m = 6: 8; ... */
        s += 1;
        pw = 2;
        for (m = 4; m <= n; m += 2) {
            s += pw;
            if (m + 2 <= n) pw *= 4;
        }
    }
    return s;
}

static seq_t a000993_count5(idx_t n) {
    seq_t s = 1;
    idx_t m0, m;
    seq_t pw;
    if (n == 0) return s;
    m0 = (n % 2 == 1) ? 1 : 2;
    pw = (m0 == 1) ? (seq_t)2 : (seq_t)10;   /* 2*5^(m0-1) */
    for (m = m0; m <= n; m += 2) {
        s += pw;
        if (m + 2 <= n) pw *= 25;
    }
    return s;
}

seq_t a(idx_t n) {
    return a000993_count2(n) * a000993_count5(n);
}
