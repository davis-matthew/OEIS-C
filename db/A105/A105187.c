/*
 * A105187  a(n) = determinant of the n X n matrix m(i,j) = (i+j+2)!/i!/j!.
 * https://oeis.org/A105187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/12)*(n+1)*(n+2)!*(n+3)! (recurrence: a(0)=1,
 *            a(n)=a(n‑1)*(n+1)*(n+2)*(n+3)/n )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
#endif

static inline seq_t compute_next(seq_t prev, idx_t i)
{
#ifdef __cplusplus
    if constexpr (std::is_integral_v<seq_t>) {
        unsigned __int128 prod = (unsigned __int128)prev * (i+1) * (i+2) * (i+3);
        return (seq_t)(prod / i);
    } else {
        seq_t prod = prev * ((seq_t)(i+1) * (seq_t)(i+2) * (seq_t)(i+3));
        return prod / (seq_t)i;
    }
#else
    unsigned __int128 prod = (unsigned __int128)prev * (i+1) * (i+2) * (i+3);
    return (seq_t)(prod / i);
#endif
}

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static size_t memo_size = 0;

    if (memo == NULL) {                /* initialiser: a(0) = 1 */
        memo_cap = 1;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;
        memo[0] = (seq_t)1;
        memo_size = 1;
    }

    if ((size_t)n >= memo_cap) {                 /* grow buffer if needed */
        size_t new_cap = memo_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        memo = tmp;
        memo_cap = new_cap;
    }

    while (memo_size <= (size_t)n) {              /* compute missing terms */
        idx_t i = memo_size;                     /* current index */
        memo[i] = compute_next(memo[i-1], i);
        ++memo_size;
    }

    return memo[n];
}
