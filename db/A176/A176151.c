/*
 * A176151  Triangle T(n, k) = 2*(n+1 - mod(n, k+1) - mod(n, n-k+1) ) + (n! -
 *          k! - (n-k)!) read by rows.
 * https://oeis.org/A176151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r, k) where T(r, k) = 2*(r+1 - (r mod (k+1)) - (r mod (r-k+1))) 
 *                   + (r! - k! - (r-k)!)
 *   r = floor((-1+sqrt(1+8*n))/2)   (the row containing the n-th element)
 *   k = n - r*(r+1)/2                (the column index within that row)
 *
 * This function returns the sequence A176151, i.e. the triangle read by rows.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#ifdef __cplusplus
/* C++ build: use a std::vector for factorial storage */
static std::vector<seq_t> fact_vec;
# define FACT_AT(i) fact_vec[(size_t)(i)]
#else
/* C build: use a malloc’ed array for factorial storage */
static seq_t *fact = NULL;
static idx_t fact_size = 0;   /* number of precomputed factorials (i.e. indices 0 .. fact_size-1) */
# define FACT_AT(i) fact[(idx_t)(i)]
#endif

seq_t a(idx_t n)
{
    /* Determine the row r and column k from the position n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Precompute factorials up to r if necessary */
#ifdef __cplusplus
    if (fact_vec.size() <= (size_t)r) {
        size_t old = fact_vec.size();
        fact_vec.resize(r + 1);
        for (size_t i = old; i <= (size_t)r; ++i)
            if (i == 0)
                fact_vec[i] = (seq_t)1;
            else
                fact_vec[i] = fact_vec[i - 1] * (seq_t)i;
    }
#else
    /* Allocate an initial factorial array if required */
    if (!fact) {
        fact = (seq_t*)malloc(sizeof(seq_t));
        if (!fact) return 0; /* allocation failure – unlikely in tests */
        fact[0] = (seq_t)1;
        fact_size = 1;
    }
    /* Resize factorial array to hold factorials up to r */
    if (fact_size <= r) {
        size_t new_size = (size_t)(r + 1);
        seq_t *tmp = (seq_t*)realloc(fact, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely in tests */
        fact = tmp;
        for (idx_t i = fact_size; i <= r; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        fact_size = new_size;
    }
#endif

    /* Compute the two parts of the formula */
    idx_t mod1 = r % (k + 1);
    idx_t mod2 = r % (r - k + 1);

    seq_t part1 = (seq_t)2 * (seq_t)(r + 1 - mod1 - mod2);
    seq_t diff   = FACT_AT(r) - FACT_AT(k) - FACT_AT(r - k);

    return part1 + diff;
}
