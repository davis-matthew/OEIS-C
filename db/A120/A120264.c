/*
 * A120264  Numerator of Sum_{k=1..n} (-1)^(k+1)/k^k.
 * https://oeis.org/A120264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ∑_{k=1..n} (-1)^(k+1)/k^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Utility headers (prototypes for the provided helper functions) */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"


static seq_t *num_cache = NULL;
static seq_t *den_cache = NULL;
static idx_t  cached_max = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cached_max)
        return;

    size_t new_size = (size_t)(n + 1);   /* we ignore index 0 */

    seq_t *new_num = alloc_seq_array(new_size);
    seq_t *new_den = alloc_seq_array(new_size);

    if (num_cache) {
        for (idx_t i = 1; i <= cached_max; ++i) {
            new_num[i] = num_cache[i];
            new_den[i] = den_cache[i];
        }
        free_seq_array(num_cache);
        free_seq_array(den_cache);
    }

    num_cache = new_num;
    den_cache = new_den;

    for (idx_t k = cached_max + 1; k <= n; ++k) {
        if (k == 1) {
            num_cache[1] = (seq_t)1;
            den_cache[1] = (seq_t)1;
            continue;
        }

        seq_t prev_num = num_cache[k - 1];
        seq_t prev_den = den_cache[k - 1];

        seq_t k_pow_k = seq_pow((seq_t)k, k);          /* k^k */
        seq_t new_num = prev_num * k_pow_k;
        if ((k & 1) == 0)          /* even k: subtract previous denom */
            new_num -= prev_den;
        else                       /* odd k: add previous denom */
            new_num += prev_den;

        seq_t new_den = prev_den * k_pow_k;

        seq_t g = seq_gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num_cache[k] = new_num;
        den_cache[k] = new_den;
    }

    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* outside the OEIS offset */
    ensure_cache(n);
    return num_cache[n];
}
