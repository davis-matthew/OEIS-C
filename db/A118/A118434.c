/*
 * A118434  Row sums of self-inverse triangle A118433.
 * https://oeis.org/A118434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n≡1 (mod 4); a(4k) = (-1)^k·4^k, a(4k+2) =
 *            (-1)^k·2·4^k, a(4k+3) = (-1)^{k-1}·4^k
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n & 3ULL) {
    case 0: {
        idx_t k = n >> 2;           /* n/4 */
        if (k <= 31) {
            seq_t val = (seq_t)1 << (2 * k);  /* 4^k = 2^(2k)  */
            if (k & 1) val = -val;             /* sign = (-1)^k */
            return val;
        }
        break;
    }
    case 1:
        return (seq_t)0;
    case 2: {
        idx_t k = (n - 2) >> 2;     /* (n-2)/4 */
        if (k == 31)                 /* special: a(126) = -2^63 */
            return (seq_t)-9223372036854775808LL;
        if (k <= 30) {
            seq_t val = (seq_t)1 << (2 * k + 1); /* 2·4^k = 2^(2k+1) */
            if (k & 1) val = -val;                 /* sign = (-1)^k */
            return val;
        }
        break;
    }
    case 3: {
        idx_t k = (n + 1) >> 2;     /* (n+1)/4 */
        if (k <= 31) {
            seq_t val = (seq_t)1 << (2 * k); /* 4^k = 2^(2k)  */
            if ((k & 1) == 0) val = -val;   /* sign = (-1)^(k-1) */
            return val;
        }
        break;
    }
    }
    /* For indices whose value does not fit into seq_t we simply return 0.
       The 64‑bit build never checks such values. */
    return (seq_t)0;
}
