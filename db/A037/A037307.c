/*
 * A037307  Numbers whose base-2 and base-9 expansions have the same digit
 *          sum.
 * https://oeis.org/A037307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑2 and base‑9 digit sums are equal. Computed
 *            by searching integers and caching the found values
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Sum of bits set in binary representation */
static seq_t binary_digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        x &= (x - 1);   /* drop lowest set bit */
        ++sum;
    }
    return sum;
}

/* Sum of digits in base‑9 representation */
static seq_t base9_digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % 9;
        x /= 9;
    }
    return sum;
}

#ifdef __cplusplus
/* C++ implementation using std::vector */
static std::vector<seq_t> terms;
static seq_t last_checked = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0 */
    while (terms.size() < (size_t)n) {
        ++last_checked;
        if (binary_digit_sum(last_checked) == base9_digit_sum(last_checked))
            terms.push_back(last_checked);
    }
    return terms[n-1];
}
#else
/* C implementation using malloc/realloc */
static seq_t *terms = NULL;
static idx_t term_count = 0;
static idx_t term_cap   = 0;
static seq_t last_checked = 0;

static void ensure_cap(idx_t need)
{
    if (need <= term_cap) return;
    idx_t new_cap = term_cap ? term_cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_terms;
    term_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0 */
    while (term_count < n) {
        ensure_cap(term_count + 1);
        seq_t candidate = ++last_checked;
        if (binary_digit_sum(candidate) == base9_digit_sum(candidate))
            terms[term_count++] = candidate;
    }
    return terms[n-1];
}
#endif
