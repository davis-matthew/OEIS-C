/*
 * A101596  Expansion of g.f. c(2*x)^4, where c(x) is the g.f. of A000108.
 * https://oeis.org/A101596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((8*n+12)/(3*n+12))*((3*n+3)/(n+3))*2^n*C(n+1)
 *            Recurrence: (n+4)*a(n) = 4*(3n+7)*a(n-1) - 16*(2n+1)*a(n-2)
 *            Initial terms: a(0)=1, a(1)=8, a(2)=56
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif

static inline seq_t compute_one(idx_t i, seq_t ap1, seq_t ap2)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 t1 = (unsigned __int128)4 * (3*i+7) * (unsigned __int128)ap1;
        unsigned __int128 t2 = (unsigned __int128)16 * (2*i+1) * (unsigned __int128)ap2;
        unsigned __int128 num = t1 - t2;
        return (seq_t)(num / (i+4));
    } else {
        seq_t t1 = seq_t(4) * seq_t(3*i+7) * ap1;
        seq_t t2 = seq_t(16) * seq_t(2*i+1) * ap2;
        seq_t num = t1 - t2;
        return num / (seq_t)(i+4);
    }
#else
    unsigned __int128 t1 = (unsigned __int128)4 * (3*i+7) * (unsigned __int128)ap1;
    unsigned __int128 t2 = (unsigned __int128)16 * (2*i+1) * (unsigned __int128)ap2;
    unsigned __int128 num = t1 - t2;
    return (seq_t)(num / (i+4));
#endif
}

seq_t a(idx_t n)
{
    /* Small fixed-size cache for typical n <= 20000 */
    static seq_t memo[20001];
    static idx_t cached = 3;   /* next index to be computed */
    static int initialized = 0;

    if (!initialized) {
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)8;
        memo[2] = (seq_t)56;
        initialized = 1;
    }

    if (n < cached) return memo[n];

    if (n < 20001) {
        for (idx_t i = cached; i <= n; ++i) {
            memo[i] = compute_one(i, memo[i-1], memo[i-2]);
        }
        cached = n + 1;
        return memo[n];
    }

    /* For n >= 20001, compute iteratively without storing all values */
    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)8;    /* a(1) */
    seq_t a2 = (seq_t)56;   /* a(2) */
    seq_t an;               /* a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        an = compute_one(i, a2, a1);
        a1 = a2;
        a2 = an;
    }
    return an;
}
