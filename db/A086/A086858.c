/*
 * A086858  Let f(n) be the inverse of the function g(x) = x^x. Then a(n) =
 *          floor(f(n)).
 * https://oeis.org/A086858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(g^-1(n)) where g(x)=x^x
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* thresholds[k] = k^k  (k = 1..15).  16^16 exceeds uint64_t,
       which is larger than any index n (idx_t == uint64_t).  */
    static const uint64_t thresholds[17] = {
        0ULL, /* unused */
        1ULL,         /* 1^1 */
        4ULL,         /* 2^2 */
        27ULL,        /* 3^3 */
        256ULL,       /* 4^4 */
        3125ULL,      /* 5^5 */
        46656ULL,     /* 6^6 */
        823543ULL,    /* 7^7 */
        16777216ULL,  /* 8^8 */
        387420489ULL, /* 9^9 */
        10000000000ULL,        /* 10^10 */
        285311670611ULL,       /* 11^11 */
        8916100448256ULL,      /* 12^12 */
        302875106592253ULL,    /* 13^13 */
        11112006825558016ULL,  /* 14^14 */
        437893890380859375ULL  /* 15^15 */
    };

    seq_t res = 1;               /* a(1)=1 is the minimal value */
    for (idx_t k = 1; k <= 15; ++k) {
        if (thresholds[k] <= (uint64_t)n) {
            res = (seq_t)k;
        } else {
            break;
        }
    }
    return res;
}
