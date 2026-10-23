/*
 * A016196  Expansion of 1/((1-10*x)*(1-12*x)).
 * https://oeis.org/A016196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*12^n - 5*10^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <vector>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    static std::vector<seq_t> pow10{1};   /* pow10[0] = 1 */
    static std::vector<seq_t> pow12{1};   /* pow12[0] = 1 */

    if (pow10.size() <= n) {
        pow10.reserve(n + 1);
        while (pow10.size() <= n)
            pow10.push_back(pow10.back() * 10);
    }
    if (pow12.size() <= n) {
        pow12.reserve(n + 1);
        while (pow12.size() <= n)
            pow12.push_back(pow12.back() * 12);
    }
    return 6 * pow12[n] - 5 * pow10[n];
#else
    static seq_t *pow10 = NULL;
    static seq_t *pow12 = NULL;
    static idx_t max_pow10 = 0;
    static idx_t max_pow12 = 0;

    if (!pow10) {
        pow10 = (seq_t*)malloc(sizeof(seq_t));   /* pow10[0] = 1 */
        pow10[0] = 1;
        max_pow10 = 0;
    }
    if (!pow12) {
        pow12 = (seq_t*)malloc(sizeof(seq_t));   /* pow12[0] = 1 */
        pow12[0] = 1;
        max_pow12 = 0;
    }

    if (n > max_pow10) {
        seq_t *tmp = (seq_t*)realloc(pow10, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed; return 0 as a safeguard */
        pow10 = tmp;
        for (idx_t i = max_pow10 + 1; i <= n; ++i)
            pow10[i] = pow10[i - 1] * 10;
        max_pow10 = n;
    }

    if (n > max_pow12) {
        seq_t *tmp = (seq_t*)realloc(pow12, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        pow12 = tmp;
        for (idx_t i = max_pow12 + 1; i <= n; ++i)
            pow12[i] = pow12[i - 1] * 12;
        max_pow12 = n;
    }

    return 6 * pow12[n] - 5 * pow10[n];
#endif
}
