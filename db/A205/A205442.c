/*
 * A205442  Least k such that n divides s(k)-s(j) for some j<k, where s(j) is
 *          the (2j-1)-st Fibonacci number.
 * https://oeis.org/A205442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides s(k)-s(j) for some j<k, *
 *            where s(i) = F_{2i-1} is the (2i−1)‑st Fibonacci number. * * The
 *            definition is equivalent to: a(n) is the first index k at which
 *            * two odd‑indexed Fibonacci numbers are congruent modulo n. *
 *            There are only n distinct residues modulo n,...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0, never used */

    /* Array of first seen indices for each residue modulo n.
       index 0 means "not yet seen". */
    idx_t *first = (idx_t*)calloc((size_t)n, sizeof(idx_t));
    if (!first)
        return 0; /* allocation failure – unlikely in test harness */

    /* Start with the first two Fibonacci numbers (F1 = 1, F2 = 1). */
    seq_t mod = (seq_t)n;
    seq_t prev = 1 % mod;   /* F1 */
    seq_t curr = 1 % mod;   /* F2 */

    /* k counts positions in the odd sub‑sequence s(k)=F_{2k-1}. */
    idx_t k = 1;
    first[(size_t)prev] = k;   /* record F1 residue */

    /* Iterate over Fibonacci indices starting from 3. */
    for (idx_t idx = 3; ; ++idx) {
        seq_t next = (prev + curr) % mod; /* F_idx */
        prev = curr;
        curr = next;

        /* When idx is odd, curr is an s(k) value. */
        if (idx & 1U) {
            ++k;                       /* update position in odd subsequence */
            seq_t residue = curr;      /* F_idx mod n */
            idx_t pos = (size_t)residue;

            if (first[pos] != 0) {
                free(first);
                return (seq_t)k;       /* first repetition found */
            }
            first[pos] = k;             /* store first occurrence */
        }
    }
}
