/*
 * A094920  a(n) = (5^n-1)/4 mod n.
 * https://oeis.org/A094920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5^n-1)/4 mod n
 * Range:     u8 n <= 63, u16 n <= 6739, u32 n <= 6739, u64 n <= 6739,
 *            u128 n <= 6739, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

#if defined(__cplusplus)

/* Helper: modular multiplication for builtin integers */
template <typename T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
static inline T mul_mod_impl(T a, T b, T mod) {
    return (T)((unsigned __int128)a * b % mod);
}

/* Helper: modular multiplication for big‑int types */
template <typename T,
          typename std::enable_if<!std::is_integral<T>::value, int>::type = 0>
static inline T mul_mod_impl(T a, T b, T mod) {
    return (a * b) % mod;
}

#else  /* C code – seq_t is a builtin integral type */

static inline seq_t mul_mod_impl(seq_t a, seq_t b, seq_t mod) {
    return (seq_t)((unsigned __int128)a * b % mod);
}

#endif

seq_t a(idx_t n) {
    /* 0 is never requested (offset starts at 1) */
    seq_t mod = (seq_t)n * 4;          /* compute 4·n safely */
    seq_t result = 1;                  /* 5^0 mod mod */
    seq_t base   = 5;                  /* base of exponentiation */

    idx_t e = n;
    while (e) {
        if (e & 1) result = mul_mod_impl(result, base, mod);
        base = mul_mod_impl(base, base, mod);
        e >>= 1;
    }

    /* t = (5^n - 1)/4    (exact division) */
    seq_t t = (result - 1) / 4;
    return t % (seq_t)n;                /* reduced modulo n */
}
