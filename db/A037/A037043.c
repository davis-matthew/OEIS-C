/*
 * A037043  3-white numbers: partition digits of n^3 into blocks of 3 starting
 *          at right; then sum of these 3-digit numbers equals n.
 * https://oeis.org/A037043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, easy, base, fini, nice
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers n such that sum of 3‑digit blocks of n³ equals n
    (OEIS A037043).  This implementation precomputes all
    solutions up to 5000, which suffices because any n > 5000
    cannot satisfy the condition.  The precomputation is
    performed lazily at the first call to a().  If the
    requested index exceeds the number of found terms,
    the function returns 0. */
#include <stdlib.h>

static seq_t *terms = NULL;    /* array holding all valid n */
static size_t term_count = 0;  /* number of terms stored */

/* enlarge 'terms' array to at least 'need' elements */
static void grow_terms(size_t need)
{
    if (need <= term_count * 2) return;
    size_t new_cap = term_count ? term_count * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!p) abort();            /* out‑of‑memory */
    terms = p;
    /* term_count remains unchanged */
}

/* precompute all solutions up to MAX_N */
static void init_terms(void)
{
    const idx_t MAX_N = 5000;   /* enough to contain all terms */
    grow_terms(MAX_N + 1);      /* allocate initial space */
    for (idx_t x = 0; x <= MAX_N; ++x) {
        seq_t cube = (seq_t)x * (seq_t)x * (seq_t)x;
        seq_t sum = 0;
        seq_t tmp = cube;
        while (tmp > 0) {
            sum += tmp % 1000;
            tmp /= 1000;
        }
        if (sum == (seq_t)x) {
            grow_terms(term_count + 1);
            terms[term_count++] = x;
        }
    }
}

seq_t a(idx_t n)
{
    if (!terms) init_terms();   /* initialize lazily */
    if (n == 0) return (seq_t)0;            /* offset is 1 */
    size_t idx = (size_t)(n - 1);
    if (idx < term_count)
        return terms[idx];
    return (seq_t)0;           /* undefined beyond known terms */
}
