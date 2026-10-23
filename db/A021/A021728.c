/*
 * A021728  Decimal expansion of 1/724.
 * https://oeis.org/A021728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in 1/724 (a(n) =
 *            ⌊10^(n+1)/724⌋ mod 10)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* cached digits 0‑9            */
    static idx_t  cache_len = 0;          /* number of digits cached      */
    static unsigned long long rem = 1;    /* current remainder (0‑723)    */

    if (n < cache_len)
        return cache[n];

    /* extend cache up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – return 0 as a safe fallback */
        return 0;
    }
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        unsigned long long t = rem * 10ULL;          /* multiply by 10   */
        seq_t d = (seq_t)(t / 724ULL);              /* next decimal digit */
        rem = t % 724ULL;                           /* new remainder    */
        cache[i] = d;
    }
    cache_len = new_len;
    return cache[n];
}
