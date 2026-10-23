/*
 * A045549  Numbers whose factorial has '6' as its final nonzero digit.
 * https://oeis.org/A045549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2091, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A045549 – Numbers whose factorial ends in a non‑zero digit 6.
    The function below generates the sequence on demand, keeping a static
    cache so repeated calls (in any order) are fast.  The last non‑zero
    digit of k! is updated incrementally: each step multiplies by k,
    removes factors of 10, and keeps only a few trailing digits to
    avoid overflow.  The computation works for any unsigned integer
    type chosen for seq_t and is also a valid C++17 program. */

#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* The modulus keeps the intermediate factorial value small while still
   being large enough to safely eliminate all trailing zeros.  The value
   1,000,000 is sufficient for the range of n needed here. */
#define FACT_MOD ((seq_t)1000000U)

/* Return the nth (1‑based) term of sequence A045549. */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined below offset; return 0 for safety */
        return (seq_t)0;

#if defined(__cplusplus)
    /* C++ implementation using std::vector */
    static std::vector<seq_t> terms;
    static idx_t cur_m = 0;      /* last processed integer  */
    static seq_t product = 1;    /* (cur_m)! with trailing zeros removed,
                                   kept modulo FACT_MOD */

    while (terms.size() < n) {
        ++cur_m;
        product *= (seq_t)cur_m;
        while (product % 10 == 0) product /= 10;
        product %= FACT_MOD;
        if (product % 10 == 6)
            terms.push_back(cur_m);
    }
    return terms[n - 1];

#else
    /* C implementation using dynamic array */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t cur_m = 0;      /* last processed integer  */
    static seq_t product = 1;    /* (cur_m)! with trailing zeros removed,
                                   kept modulo FACT_MOD */

    while (terms_len < (size_t)n) {
        ++cur_m;
        product *= (seq_t)cur_m;
        while (product % 10 == 0) product /= 10;
        product %= FACT_MOD;
        if (product % 10 == 6) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_arr) return (seq_t)0;  /* allocation failed */
                terms = new_arr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cur_m;
        }
    }
    return terms[n - 1];
#endif
}
