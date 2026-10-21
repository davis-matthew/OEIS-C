/*
 * A076772  Even numbers n representable as the sum of 2 coprime odd
 *          composites.
 * https://oeis.org/A076772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A076772: even numbers representable as the sum of two coprime odd composites.
 * The sequence is defined by a(n) = the n-th even integer e such that there exist odd
 * composite numbers a,b with a+b=e and gcd(a,b)=1.
 *
 * Implementation details:
 *   - Generate the list of odd composite numbers up to 211 once (all required values
 *     for e<212).  The list remains static and is reused for every call.
 *   - For an even number e<212, test all decompositions a+b=e where a and b are odd
 *     composites.  If a coprime pair is found the number is included.
 *   - For e>=212 all even numbers are known to be representable, so no search is
 *     needed once this threshold is passed.
 *   - The function iterates over even integers starting at 34, counting those that
 *     satisfy the condition until the desired index n is reached.
 *
 * The algorithm is O(n * k) where k is the number of odd composites below 212
 * (k≈70).  For n up to 10 000 the runtime easily stays below 2 s.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"   /* bool isPrimeTrial(seq_t n); */
#include "../utils/gcd.h"            /* seq_t gcd(seq_t a, seq_t b); */

/* --------------------------------------------------------------------------- */
/* Static data: odd composite numbers up to 211 (the largest relevant odd composite) */
static seq_t odd_composites[120];
static size_t odd_composite_count = 0;

/* Build the table of odd composites once. */
static void build_odd_composites(void)
{
    if (odd_composite_count > 0) return;      /* already built */

    size_t idx = 0;
    for (seq_t n = 9; n <= 211; n += 2) {     /* odd numbers only */
        if (!isPrimeTrial(n)) {
            odd_composites[idx++] = n;
        }
    }
    odd_composite_count = idx;
}

/* Test whether an even number e < 212 is representable as a sum of two coprime
 * odd composite numbers. */
static bool is_representable_even(seq_t e)
{
    /* e is even and < 212, so iterate over all decompositions a+b=e. */
    for (size_t i = 0; i < odd_composite_count; ++i) {
        seq_t a = odd_composites[i];
        if (a > e - 9) break;          /* minimal odd composite is 9 */
        seq_t b = e - a;
        if (b < 9 || (b & 1) == 0) continue;          /* b must be odd composite */
        if (isPrimeTrial(b)) continue;                /* b must be composite */

        if (gcd(a, b) == 1) return true;               /* coprime pair found */
    }
    return false;
}

/* --------------------------------------------------------------------------- */
/* Main sequence function required by the build system */
seq_t a(idx_t n)
{
    /* Ensure the odd composite table is ready. */
    build_odd_composites();

    seq_t e = 34;               /* first term of the sequence */
    seq_t idx = 0;              /* 0‑based index of the current term */

    for (;;) {
        if ((e >= 212) || is_representable_even(e)) {
            if (idx == n) {
                return e;
            }
            ++idx;
        }
        e += 2;    /* only even numbers can be terms of the sequence */
    }
}
