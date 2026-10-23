/*
 * A117604  Padovan numbers which can be divided by their multiplicative
 *          digital root.
 * https://oeis.org/A117604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Padovan numbers that are divisible by their multiplicative digital root.
   The sequence a(n) consists of the distinct Padovan numbers P(k) such that
   P(k) mod MR(P(k)) == 0, where MR(x) is the multiplicative digital root of x.
   Recurrence: P(0)=P(1)=P(2)=1, P(k)=P(k-2)+P(k-3) for k≥3. */

#include <stdbool.h>

/* Return the multiplicative digital root of x.
   If any digit of the intermediate product is 0, the root is 0. */
static seq_t multiplicative_digital_root(seq_t x)
{
    for (;;) {
        if (x < (seq_t)10) {
            return x;          /* single‑digit root (could be 0) */
        }
        seq_t prod = (seq_t)1;
        seq_t tmp  = x;
        while (tmp > (seq_t)0) {
            seq_t digit = tmp % (seq_t)10;
            if (digit == (seq_t)0) {
                return (seq_t)0;          /* zero digit destroys the root */
            }
            prod *= digit;
            tmp   /= (seq_t)10;
        }
        x = prod;
    }
}

/* Return the n-th element of the OEIS sequence A117604. */
seq_t a(idx_t n)
{
    /* Padovan state variables: P(k-3), P(k-2), P(k-1). */
    seq_t p0 = (seq_t)1;   /* P(0) */
    seq_t p1 = (seq_t)1;   /* P(1) */
    seq_t p2 = (seq_t)1;   /* P(2) */

    idx_t   iter     = (idx_t)0;   /* iteration counter for generation */
    seq_t   current;              /* current Padovan number */
    seq_t   prev     = (seq_t)0;   /* last distinct Padovan number seen */
    idx_t   count    = (idx_t)0;   /* number of qualifying terms found */

    for (;;) {
        /* Generate the next Padovan number in order. */
        if (iter == (idx_t)0) {
            current = p0;
        } else if (iter == (idx_t)1) {
            current = p1;
        } else if (iter == (idx_t)2) {
            current = p2;
        } else {                     /* iter ≥ 3 */
            /* P(k) = P(k-2) + P(k-3) */
            seq_t next = p0 + p1;
            p0 = p1;
            p1 = p2;
            p2 = next;
            current = next;
        }

        /* Skip duplicate Padovan values (they appear consecutively). */
        if (current != prev) {
            seq_t root = multiplicative_digital_root(current);
            if (root != (seq_t)0 && current % root == (seq_t)0) {
                count++;
                if (count == n) {
                    return current;
                }
            }
            prev = current;
        }

        iter++;
    }
}
