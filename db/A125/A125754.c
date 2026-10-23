/*
 * A125754  Numbers n whose reverse binary representation has the following
 *          property: let a 0 mean "halving" and a 1 mean "k -> 3k+1". The
 *          number describes an operation k -> f_n(k). If the equation f_n(k)
 *          = k has an integer solution, n is a term in the sequence.
 * https://oeis.org/A125754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 88, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A125754 – numbers whose reverse binary representation encodes a linear
    map f(k) = a*k + b with a non‑zero rational coefficient.
    We compute the sequence by brute force search: iterate over
    successive positive integers, test the property, and return the
    n‑th one found. */

/* We need the standard integer and boolean types. */
#include <stdint.h>
#include <stdbool.h>

/* helper: test whether a given integer value satisfies the
   condition that (b/(1-a)) is an integer. */
static bool is_term(uint64_t val)
{
    /* a_num / D  == a  ,  b_num / D  == b
       D grows by factor 2 for each zero bit; only
       odd bits multiply a_num and add D to b_num. */
    unsigned __int128 a_num = 1;   /* numerator of a */
    unsigned __int128 D     = 1;   /* common denominator of a and b */
    unsigned __int128 B     = 0;   /* numerator of b */

    uint64_t x = val;
    while (x) {
        if (x & 1U) {                 /* bit = 1 : apply 3k+1 */
            a_num = a_num * 3;
            B = B * 3 + D;            /* b = 3b + 1  ->  (3B + D)/D  */
        } else {                      /* bit = 0 : apply ½ */
            D = D * 2;                /* denominator doubles */
            /* a_num and B stay unchanged (division by 2) */
        }
        x >>= 1;
    }

    /* diff = |D - a_num|.  The fixed‑point equation is
       k = B / (D - a_num).  An integer solution exists iff
       B is divisible by that difference. */
    unsigned __int128 diff;
    if (D >= a_num)
        diff = D - a_num;
    else
        diff = a_num - D;

    /* diff is never zero for any positive val (a cannot be 1). */
    return (B % diff) == 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset is 1, but guard anyway */

    uint64_t candidate = 1;   /* integers to test */
    idx_t    count = 0;       /* how many terms found so far */

    while (true) {
        if (is_term(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
