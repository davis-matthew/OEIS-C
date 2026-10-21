/*
 * A052133  CONTINUANT transform of 0, 1, 1, 2, 1, 3, 2, 3, ... (A002487).
 * https://oeis.org/A052133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stern(n) * a(n-1) + a(n-2), with a(0)=0, a(1)=1 (Stern(n)
 *            is the Stern's diatomic sequence)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 34, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    if (n == 1u)
        return 1;

    size_t N = (size_t)n;
    seq_t *s;
#ifdef __cplusplus
    std::vector<seq_t> vec(N + 1);
    s = vec.data();
#else
    s = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!s)
        return 0; /* allocation failure */
#endif
    s[0] = 0;
    s[1] = 1;
    for (size_t i = 2; i <= N; ++i) {
        if ((i & 1u) == 0u) {
            s[i] = s[i >> 1];
        } else {
            size_t k = i >> 1;           /* (i-1)/2 */
            s[i] = s[k] + s[k + 1];
        }
    }

    seq_t a0 = 0;
    seq_t a1 = 1;
    seq_t ac;
    for (size_t i = 2; i <= N; ++i) {
        ac = s[i] * a1 + a0;
        a0 = a1;
        a1 = ac;
    }

#ifdef __cplusplus
    /* vec is freed automatically */
#else
    free(s);
#endif
    return a1;
}
