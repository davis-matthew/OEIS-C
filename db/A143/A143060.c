/*
 * A143060  a(n) = (n^5 + 145*n^4 + 905*n^3 + 155*n^2 + 594*n + 120)/120.
 * https://oeis.org/A143060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 145*n^4 + 905*n^3 + 155*n^2 + 594*n + 120)/120
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 196, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, etc., if needed by the build */

static inline seq_t binom_u64(idx_t n, idx_t k) {
    if (k > n)
        return 0;
    /* compute n*(n-1)*...*(n-k+1) / k! using 128‑bit intermediate */
    unsigned __int128 prod = 1;
    for (idx_t i = 0; i < k; ++i)
        prod *= (unsigned __int128)(n - i);
    switch (k) {
    case 0: return 1;
    case 1: return (seq_t)prod;
    case 2: return (seq_t)(prod / 2);
    case 3: return (seq_t)(prod / 6);
    case 4: return (seq_t)(prod / 24);
    case 5: return (seq_t)(prod / 120);
    default: return 0; /* should not happen for this sequence */
    }
}

seq_t a(idx_t n) {
    seq_t res = 1;             /* 1 + ... */
    res += (seq_t)15 * (seq_t)n;

    seq_t c2 = binom_u64(n, 2);    /* C(n,2) */
    seq_t c3 = binom_u64(n, 3);    /* C(n,3) */
    seq_t c4 = binom_u64(n, 4);    /* C(n,4) */
    seq_t c5 = binom_u64(n, 5);    /* C(n,5) */

    res += (seq_t)65 * c2;
    res += (seq_t)90 * c3;
    res += (seq_t)31 * c4;
    res += c5;                         /* C(n,5) term */
    return res;
}
