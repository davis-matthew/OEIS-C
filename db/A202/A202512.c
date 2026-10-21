/*
 * A202512  Number of zero-sum -n..n arrays of 4 elements with first through
 *          third differences also in -n..n.
 * https://oeis.org/A202512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            -a(n-1)+2*a(n-3)+4*a(n-4)+2*a(n-5)-a(n-6)-5*a(n-7)-5*a(n-8)-a(n-9)+2*a(n-10)+4*a(n-11)+2*a(n-12)-a(n-14)-a(n-15)
 *            Initial values: a(1)=1, a(2)=15, a(3)=29, a(4)=75, a(5)=115,
 *            a(6)=217, a(7)=297, a(8)=479, a(9)=609, a(10)=887, a(11)=1091,
 *            a(12)=1493, a(13)=1765, a(14)=2315, a(15)=2689
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_comp = 0;

    /* Initial values for n = 1 .. 15 */
    static const seq_t init_vals[15] = {
        (seq_t)1,  (seq_t)15, (seq_t)29, (seq_t)75, (seq_t)115,
        (seq_t)217, (seq_t)297, (seq_t)479, (seq_t)609, (seq_t)887,
        (seq_t)1091, (seq_t)1493, (seq_t)1765, (seq_t)2315, (seq_t)2689
    };
    const size_t init_len = 15;

    /* Ensure cache space for current request */
    if (n + 1 > cap) {
        size_t new_cap = (n + 1) > (cap * 2) ? (n + 1) : (cap * 2);
        if (new_cap == 0) new_cap = (n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        /* Zero new slots */
        if (new_cap > cap)
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        if (i <= (idx_t)init_len) {
            cache[i] = init_vals[i - 1];
        } else {
            seq_t sum = 0;
            sum -= cache[i - 1];
            sum += (seq_t)2 * cache[i - 3];
            sum += (seq_t)4 * cache[i - 4];
            sum += (seq_t)2 * cache[i - 5];
            sum -= cache[i - 6];
            sum -= (seq_t)5 * cache[i - 7];
            sum -= (seq_t)5 * cache[i - 8];
            sum -= cache[i - 9];
            sum += (seq_t)2 * cache[i - 10];
            sum += (seq_t)4 * cache[i - 11];
            sum += (seq_t)2 * cache[i - 12];
            sum -= cache[i - 14];
            sum -= cache[i - 15];
            cache[i] = sum;
        }
    }
    last_comp = (last_comp < n) ? n : last_comp;

    return cache[n];
}
