/*
 * A018410  Divisors of 356.
 * https://oeis.org/A018410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: all positive divisors of 356 in ascending order */

#include <stdbool.h>
#include <stdlib.h>

/* Helper that constructs the list of divisors of 356.
   The result is stored in a static array that is allocated only once. */
static void init_divisors(seq_t **divs, size_t *len)
{
    static bool done = false;
    static seq_t *list = NULL;
    static size_t size = 0;
    if (done) {
        *divs = list;
        *len  = size;
        return;
    }

    /* Temporary arrays to collect the “small” and “large” half of the divisors.
       The number of divisors of 356 is tiny, so 16 elements are more than enough. */
    seq_t small[16], large[16];
    size_t small_len = 0, large_len = 0;
    const idx_t N = 356;

    for (idx_t i = 1; (idx_t)i * (idx_t)i <= N; ++i) {
        if (N % i == 0) {
            small[small_len++] = (seq_t)i;
            if (i != N / i)
                large[large_len++] = (seq_t)(N / i);
        }
    }

    /* Allocate a single contiguous array that will hold the divisors in
       increasing order: small ascending, then large descending. */
    size_t total = small_len + large_len;
    list = (seq_t*)malloc(total * sizeof(seq_t));
    size_t pos = 0;
    for (size_t j = 0; j < small_len; ++j)
        list[pos++] = small[j];
    for (size_t j = 0; j < large_len; ++j)
        list[pos++] = large[large_len - 1 - j];

    done  = true;
    size  = total;
    *divs = list;
    *len  = total;
}

/* Return the n‑th divisor of 356 (1‑based).  If n is out of range,
   0 is returned. */
seq_t a(idx_t n)
{
    static seq_t *divs = NULL;
    static size_t divs_len = 0;
    if (!divs)
        init_divisors(&divs, &divs_len);

    if (n < 1 || n > (idx_t)divs_len)
        return 0;
    return divs[n - 1];
}
