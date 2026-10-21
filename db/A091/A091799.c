/*
 * A091799  a(1) = 3. To get a(n+1), write the string a(1)a(2)...a(n) as xy^k
 *          for words x and y (where y has positive length) and k is
 *          maximized, i.e., k = the maximal number of repeating blocks at the
 *          end of the sequence so far. Then a(n+1) = max(k,3).
 * https://oeis.org/A091799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = max(k, 3), where k is the maximum number of consecutive *
 *            copies of a word y that appear at the end of the decimal string
 *            * a(1) a(2) ... a(n). a(1) = 3. The function returns a(n) for n
 *            ≥ 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_value, compute_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/append_value.h"
#include "../utils/compute_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 just in case */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static char *concat = NULL;
    static size_t concat_len = 0;
    static size_t concat_cap = 0;
    static idx_t computed = 0;   /* number of terms already computed */

    if (n > cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 64;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }

    while (computed < n) {
        seq_t val;
        if (computed == 0) {
            val = (seq_t)3;          /* a(1) = 3 */
        } else {
            seq_t k = compute_k(concat, concat_len);
            val = (k > (seq_t)3) ? k : (seq_t)3;
        }
        if (computed >= cache_len) cache_len = computed + 1;
        cache[computed] = val;
        append_value(&concat, &concat_cap, &concat_len, val);
        ++computed;
    }
    return cache[n - 1];
}
