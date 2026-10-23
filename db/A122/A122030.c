/*
 * A122030  See Comments lines for definition.
 * https://oeis.org/A122030
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   read_decimal_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = read(16, B1_(B2_(B3_(..._Bm))), for n = m+4,
 * where the sequence of bases Bk = 32 * 2^(k-1).
 * The computation takes the decimal digits of the number to be read,
 * and interprets them in the specified base, treating the digits
 * as if they were written in that base.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/read_decimal_in_base.h"

/* cached base values for each chain length m (m >= 1) */
static seq_t *base_cache = NULL;
static idx_t base_cache_size = 0;

/* ensure the cache has room for m */
static void ensure_base_cache(idx_t m)
{
    if (m >= base_cache_size) {
        idx_t new_size = (m + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(base_cache, new_size * sizeof(seq_t));
        for (idx_t i = base_cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        base_cache = new_cache;
        base_cache_size = new_size;
    }
}

/* compute the nested base value for chain length m (m >= 1) */
static seq_t compute_base(idx_t m)
{
    if (m == 0) return 0;           /* not used for n <= 4 */
    ensure_base_cache(m);
    if (base_cache[m] != 0) return base_cache[m];

    /* Build array of base values B[1..m]  (B[k] = 32 * 2^(k-1)) */
    seq_t *B = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    seq_t val = (seq_t)32;
    for (idx_t k = 1; k <= m; ++k) {
        B[k] = val;
        val = val * 2;
    }

    seq_t base_val;
    if (m == 1) {
        base_val = B[1];
    } else {
        /* innermost: B[m-1] in base B[m] */
        base_val = read_decimal_in_base(B[m - 1], B[m]);
        /* climb outward */
        for (idx_t k = m - 2; k >= 1; --k) {
            base_val = read_decimal_in_base(B[k], base_val);
        }
    }

    free(B);
    base_cache[m] = base_val;
    return base_val;
}

/* cache for a(n) values */
static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

static void ensure_a_cache(idx_t n)
{
    if (n >= a_cache_size) {
        idx_t new_size = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        for (idx_t i = a_cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        a_cache = new_cache;
        a_cache_size = new_size;
    }
}

/* main function to compute the sequence value */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;         /* undefined for indices below offset */

    if (n == 4)
        return (seq_t)16;

    ensure_a_cache(n);
    if (a_cache[n] != 0)
        return a_cache[n];

    idx_t m = n - 4;            /* chain length */
    seq_t base_val = compute_base(m);
    seq_t result = read_decimal_in_base((seq_t)16, base_val);

    a_cache[n] = result;
    return result;
}
