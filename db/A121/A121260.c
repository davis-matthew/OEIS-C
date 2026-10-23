/*
 * A121260  Beginning with a(1)=0, take a(n) to be the smallest number not
 *          equal to the product of two consecutive (not distinct) earlier
 *          terms.
 * https://oeis.org/A121260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for even n, a(n)=a(n-1); for odd n,
   a(n) = the smallest integer > a(n-1) that is not equal to a(i)*a(i-1)
   for any i<n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* static state ----------------------------------------------------------- */

static idx_t  max_computed = 0;          /* largest n already computed */
static seq_t *seq          = NULL;       /* array of a(n), 1‑based */
static size_t seq_cap      = 0;          /* capacity for seq */

static seq_t *products     = NULL;       /* stored products a(i)*a(i-1) */
static size_t prod_len     = 0;          /* number of products stored */
static size_t prod_cap     = 0;          /* capacity for products */

/* ----------------------------------------------------------------------- */

static void ensure_seq_cap(idx_t need)
{
    /* need is the maximum index that must be valid (1‑based) */
    size_t required = (size_t)need + 1;          /* +1 to keep index 0 unused */
    if (seq_cap >= required) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < required) new_cap <<= 1;

    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    if (seq) {
        for (size_t i = 1; i <= seq_cap; ++i) {
            new_arr[i] = seq[i];
        }
#ifdef __cplusplus
        delete [] seq;
#else
        free(seq);
#endif
    }

    seq       = new_arr;
    seq_cap   = new_cap;
}

static void ensure_prod_cap(size_t need)
{
    if (prod_cap >= need) return;

    size_t new_cap = prod_cap ? prod_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    if (products) {
        for (size_t i = 0; i < prod_len; ++i) {
            new_arr[i] = products[i];
        }
#ifdef __cplusplus
        delete [] products;
#else
        free(products);
#endif
    }

    products  = new_arr;
    prod_cap  = new_cap;
}

static bool product_used(seq_t value)
{
    for (size_t i = 0; i < prod_len; ++i) {
        if (products[i] == value) return true;
    }
    return false;
}

static void add_product(seq_t value)
{
    ensure_prod_cap(prod_len + 1);
    products[prod_len++] = value;
}

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    if (n > max_computed) {
        /* Compute terms up to n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            ensure_seq_cap(i);

            if (i == 1) {
                seq[1] = (seq_t)0;          /* a(1) = 0 */
            } else if ((i & 1) == 0) {      /* even i */
                seq[i] = seq[i - 1];
            } else {                       /* odd i */
                seq_t cand = seq[i - 1] + (seq_t)1;
                while (product_used(cand)) {
                    cand = cand + (seq_t)1;
                }
                seq[i] = cand;
            }

            /* record product a(i)*a(i-1) if defined */
            if (i > 1) {
                seq_t ai   = seq[i];
                seq_t aip1 = seq[i - 1];
                seq_t prod;

                if (ai == 0 || aip1 == 0) {
                    prod = (seq_t)0;
                    add_product(prod);
                } else {
                    prod = ai * aip1;
                    /* detect overflow for fixed‑width types */
#ifdef __SIZEOF_INT128__
                    /* seq_t may be bigger than 128 bits, rely on built‑in accurate */
#else
                    if (prod / ai != aip1) {
                        /* overflow detected – skip adding this product */
                        continue;
                    }
#endif
                    add_product(prod);
                }
            }
        }
        max_computed = n;
    }
    return seq[n];
}
