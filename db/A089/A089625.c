/*
 * A089625  Replace 2^k in binary expansion of n with (k+1)-st prime.
 * https://oeis.org/A089625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = sum over bit positions k of n of prime(k+1), where
 *            prime(1)=2, prime(2)=3, prime(3)=5, ... a(1) = 2 a(2) = 3 a(3) =
 *            5 a(4) = 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _a089625_primes[64] = {
    2,   3,   5,   7,   11,  13,  17,  19,
    23,  29,  31,  37,  41,  43,  47,  53,
    59,  61,  67,  71,  73,  79,  83,  89,
    97,  101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t bit = 0;
    while (n) {
        if (n & 1) {
            sum += _a089625_primes[bit];
        }
        n >>= 1;
        bit++;
    }
    return sum;
}
