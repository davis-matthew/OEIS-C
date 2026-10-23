/*
 * A043261  Sum of the binary digits of the n-th base-2 palindrome.
 * https://oeis.org/A043261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0, a(2)=1. Let b(1)=0, b(2)=1, b(2k)=b(k), b(2k-1)=b(k-1).
 *            Let c(1)=0, c(2)=1, c(2k)=c(k), c(2k-1)=c(k-1)+1. Then for n≥3:
 *            if n is even : a(n)=2*c(n) if n is odd : a(n)=2*c(n)-b(n) (This
 *            is equivalent to the OEIS recurrence for A043261)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _b(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if ((n & 1ULL) == 0) {          /* n even: b(n)=b(n/2) */
        return _b(n / 2);
    } else {                        /* n odd: b(n)=b((n+1)/2 - 1) */
        idx_t m = (n + 1) / 2;      /* m = ceil(n/2) */
        return _b(m - 1);
    }
}

static seq_t _c(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if ((n & 1ULL) == 0) {          /* n even: c(n)=c(n/2) */
        return _c(n / 2);
    } else {                        /* n odd: c(n)=c((n+1)/2 - 1)+1 */
        idx_t m = (n + 1) / 2;
        return _c(m - 1) + (seq_t)1;
    }
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t cn = _c(n);
    if ((n & 1ULL) == 0) {              /* even index */
        return cn << 1;                 /* 2*c(n) */
    } else {                           /* odd index */
        seq_t bn = _b(n);
        return (cn << 1) - bn;          /* 2*c(n) - b(n) */
    }
}
