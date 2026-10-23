/*
 * A022807  a(n) = S(n) + c(n) where S(n) = [ n*sqrt(2) ] + [ n*sqrt(3) ] and
 *          c is the complement of S.
 * https://oeis.org/A022807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(n) + c(n) where S(n) = ⌊n∙√2⌋ + ⌊n∙√3⌋ and c(n) is the
 *            n‑th positive integer not belonging to the set {S(k)}
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* compute S(k) = floor(k*sqrt(2)) + floor(k*sqrt(3)) */
static inline seq_t compute_S(idx_t k)
{
    seq_t kseq = (seq_t)k;
    seq_t sq   = kseq * kseq;          /* k², safe in seq_t */
    seq_t t1   = floor_sqrt(sq * 2);   /* floor(k*sqrt(2)) */
    seq_t t2   = floor_sqrt(sq * 3);   /* floor(k*sqrt(3)) */
    return t1 + t2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety guard – offset starts at 1 */

    /* S(n) */
    seq_t Sn = compute_S(n);

    /* find the n‑th missing number (c(n)) */
    idx_t      missing = 0;        /* how many missing numbers seen so far */
    seq_t      prevS   = 0;        /* last S value processed */
    idx_t      i       = 1;        /* current index for generating S(i) */
    seq_t      c_val   = 0;

    while (1) {
        seq_t curS = compute_S(i);              /* S(i) */
        seq_t gap  = curS - prevS - 1;          /* numbers between prevS and curS */

        /* If the n‑th missing lies in this gap, compute it */
        seq_t missing_seq = (seq_t)missing;
        if (missing_seq + gap >= (seq_t)n) {
            idx_t offset = n - missing;         /* position inside the gap */
            c_val = prevS + (seq_t)offset;      /* nth missing number */
            break;
        }

        /* Otherwise consume the entire gap and continue */
        missing += (idx_t)gap;   /* gap < n, safe to cast */
        prevS    = curS;
        ++i;
    }

    return Sn + c_val;     /* a(n) = S(n) + c(n) */
}
