/*
 * A167121  a(n) = 20*a(n-1) - 64*a(n-2) + 2 for n > 2; a(0) = 1, a(1) = 22,
 *          a(2) = 377.
 * https://oeis.org/A167121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2) + 2 for n>2; Initial values:
 *            a(0)=1, a(1)=22, a(2)=377
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>

/* Helper to compute the next term using large intermediate when seq_t is integral. */
template <typename T>
static T compute_next(T a2, T a1)
{
    if constexpr (std::is_integral_v<T>) {
        using Big = unsigned __int128;
        Big temp = (Big)20 * a2 - (Big)64 * a1 + 2;
        return (T)temp;
    } else {
        /* Assume seq_t is a big‑int class with normal arithmetic operators. */
        return (T)(20 * a2 - 64 * a1 + 2);
    }
}
#else
/* For C: seq_t is an integral type. Use a 128‑bit intermediate if available. */
static inline seq_t compute_next(seq_t a2, seq_t a1)
{
#ifdef __SIZEOF_INT128__
    unsigned __int128 temp = (unsigned __int128)20 * a2 - (unsigned __int128)64 * a1 + 2;
    return (seq_t)temp;
#else
    unsigned long long temp = (unsigned long long)20 * a2 - (unsigned long long)64 * a1 + 2;
    return (seq_t)temp;
#endif
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)377;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)22;  /* a(1) */
    seq_t curr  = (seq_t)377; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = compute_next(curr, prev1);
        prev2 = prev1;
        prev1 = curr;
        curr = next;
    }
    return curr;
}
