/*
 * A109307  Numbers m such that m^2 + (m+/-1)^2 are both semiprimes.
 * https://oeis.org/A109307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers m such that m^2+(m-1)^2 and m^2+(m+1)^2 are both semiprimes.
   The function generates the sequence by brute‑force search, memoising
   found terms so that calls in any order are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_semiprime_uint64.h"

#ifdef __cplusplus
#include <vector>
using std::vector;
#endif

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1; n==0 is undefined */
        return 0;

    /* state kept between calls */
    static idx_t max_m = 0;     /* last m that was tested */
    static idx_t count = 0;     /* number of terms found so far */

#ifdef __cplusplus
    static vector<seq_t> terms;
#else
    static seq_t *terms = NULL;
    static idx_t terms_cap = 0;
#endif

    /* Keep generating terms until we have at least n of them */
    while (count < n) {
        /* test next m */
        ++max_m;

        /* compute both sums using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 t = (unsigned __int128)max_m * max_m;
        uint64_t sum1 = (uint64_t)(2 * t + 2 * (unsigned __int128)max_m + 1);
        uint64_t sum2 = (uint64_t)(2 * t - 2 * (unsigned __int128)max_m + 1);

        if (is_semiprime_uint64(sum1) && is_semiprime_uint64(sum2)) {
#ifdef __cplusplus
            terms.push_back((seq_t)max_m);
#else
            if (terms_cap <= count) {
                idx_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *new_terms = realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) exit(EXIT_FAILURE);
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[count] = (seq_t)max_m;
#endif
            ++count;
        }
    }

    return terms[n - 1];
}
