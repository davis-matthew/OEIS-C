/*
 * A044452  Numbers n such that string 2,2 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer m ≥ 0 such that base‑4(m) contains the
 *            substring "22" and base‑4(m+1) does NOT contain "22"
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* return true iff the base‑4 representation of x contains "22" */
static bool has22(seq_t x)
{
    unsigned prev_is2 = 0;          /* whether the previous digit was 2   */
    while (x > 0) {
        unsigned digit = (unsigned)(x % 4);
        if (prev_is2 && digit == 2)
            return true;
        prev_is2 = (digit == 2);
        x /= 4;
    }
    return false;
}

/* external linkage – the sequence function */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* array of found terms */
    static size_t size   = 0;      /* number of terms stored */
    static size_t cap    = 0;      /* allocated capacity */
    static seq_t next    = 0;      /* next integer to test */

    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    if (n <= size)
        return terms[n-1];

    /* extend the sequence until we have n terms */
    while (size < (size_t)n) {
        seq_t cand = next;
        ++next;                   /* prepare for next candidate */

        if (has22(cand) && !has22(cand + 1)) {
            if (size == cap) {    /* grow the array */
                cap = cap ? cap * 2 : 32;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[size++] = cand;
        }
    }
    return terms[n-1];
}
