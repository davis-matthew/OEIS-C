/*
 * A072881  a(1)=a(2)=a(3)=1; for n>3,
 *          a(n)=(a(n-1)*a(n-2)+a(n-1)+a(n-2))/a(n-3).
 * https://oeis.org/A072881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (a(n-1)*a(n-2)+a(n-1)+a(n-2))/a(n-3), a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Multiplication by 14 with overflow safety for integral seq_t types. */
#if defined(__cplusplus) && (__cplusplus >= 201703L)
# include <type_traits>
static inline seq_t mul14(seq_t x) {
    if constexpr (std::is_integral<seq_t>::value) {
        return seq_t((unsigned __int128) x * 14);
    } else {
        return x * (seq_t)14;
    }
}
#else
static inline seq_t mul14(seq_t x) {
    return (seq_t)((unsigned __int128) x * 14);
}
#endif

/* Internal storage for odd and even subsequences.                      */
/* odd[k]   = a(2*k-1)   (k >= 1)                                      */
/* even[k]  = a(2*k)     (k >= 1)                                      */
static seq_t *odd  = NULL;
static idx_t  odd_cap  = 0;
static idx_t  odd_len  = 0;   /* highest index computed */

static seq_t *even = NULL;
static idx_t  even_cap  = 0;
static idx_t  even_len  = 0;   /* highest index computed */

/* Ensure the odd array can hold index k and compute missing terms */
static void ensure_odd(idx_t k)
{
    if (k <= odd_len) return;

    if (!odd) {
        odd_cap = 4;
        odd = (seq_t *)calloc(odd_cap + 1, sizeof(seq_t));
        /* initial values for a(1), a(3), a(5) */
        odd[1] = (seq_t)1;
        odd[2] = (seq_t)1;
        odd[3] = (seq_t)7;
        odd_len = 3;
    }
    if (k <= odd_len) return;

    idx_t new_cap = odd_cap ? odd_cap : 4;
    while (new_cap < k) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(odd, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) exit(1);                /* fatal, but unlikely */
    odd = tmp;
    odd_cap = new_cap;
    for (idx_t i = odd_len + 1; i <= k; ++i) {
        seq_t v = mul14(odd[i-1]) - mul14(odd[i-2]) + odd[i-3];
        odd[i] = v;
    }
    odd_len = k;
}

/* Ensure the even array can hold index k and compute missing terms */
static void ensure_even(idx_t k)
{
    if (k <= even_len) return;

    if (!even) {
        even_cap = 4;
        even = (seq_t *)calloc(even_cap + 1, sizeof(seq_t));
        /* initial values for a(2), a(4), a(6) */
        even[1] = (seq_t)1;
        even[2] = (seq_t)3;
        even[3] = (seq_t)31;
        even_len = 3;
    }
    if (k <= even_len) return;

    idx_t new_cap = even_cap ? even_cap : 4;
    while (new_cap < k) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(even, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) exit(1);                /* fatal, but unlikely */
    even = tmp;
    even_cap = new_cap;
    for (idx_t i = even_len + 1; i <= k; ++i) {
        seq_t v = mul14(even[i-1]) - mul14(even[i-2]) + even[i-3];
        even[i] = v;
    }
    even_len = k;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n <= 3) return (seq_t)1;

    if (n & 1) {           /* odd index */
        idx_t k = (n + 1) >> 1;
        ensure_odd(k);
        return odd[k];
    } else {               /* even index */
        idx_t k = n >> 1;
        ensure_even(k);
        return even[k];
    }
}
