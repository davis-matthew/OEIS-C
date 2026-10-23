/*
 * A127174  Numbers n of the form 3*k such that partition number of n is also
 *          of the form 3*k.
 * https://oeis.org/A127174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127174 – the nth positive integer of the form 3·k whose partition
 * number p(3·k) is also divisible by 3.  The sequence is generated on
 * demand by computing partition numbers modulo 3 and keeping a list of
 * qualifying multiples of three.  This implementation uses only
 * integer arithmetic and standard C runtime facilities.  It works for
 * any width of seq_t (built-in integers or arbitrary‑precision classes)
 * and compiles as C11 or C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Storage for partition numbers modulo 3.  part_mod[n] == p(n) mod 3. */
static uint8_t *part_mod   = NULL;  /* array of size part_capacity */
static idx_t   part_capacity = 0;   /* allocated size in elements */
static idx_t   part_len      = 0;   /* number of elements currently valid
                                     (last computed index + 1) */

/* Storage for the qualifying sequence values (indices of the form 3·k). */
static idx_t *qual   = NULL;  /* array of qualifying numbers */
static size_t qual_len = 0;    /* number of terms stored so far */
static size_t qual_cap = 0;    /* current allocation size of qual */

/* Largest n that has been examined for qualification. */
static idx_t last_checked = 0;

/* Extend the array holding partition numbers modulo 3 up to index n. */
static void extend_partitions(idx_t n)
{
    if (part_len > n) return;       /* already have enough */

    /* Initialise storage if first time. */
    if (part_len == 0) {
        size_t cap = 8;
        part_mod = (uint8_t *)realloc(part_mod, cap * sizeof(uint8_t));
        if (!part_mod) exit(1);
        part_capacity = cap;
        part_mod[0] = 1;           /* p(0) = 1 */
        part_len = 1;
    }

    /* Grow storage if necessary. */
    if ((size_t)(n + 1) > part_capacity) {
        size_t new_cap = part_capacity;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(part_mod, new_cap * sizeof(uint8_t));
        if (!tmp) exit(1);
        part_mod = tmp;
        part_capacity = new_cap;
    }

    /* Compute p(k) mod 3 for k = part_len … n using Euler's pentagonal
     * recurrence.  The recurrence is p(0)=1 and for k>0:
     *   p(k) = Σ (-1)^(t-1) (p(k - g1) + p(k - g2))
     * where g1 = t(3t-1)/2, g2 = t(3t+1)/2.  All operations are done
     * modulo 3. */
    for (idx_t k = part_len; k <= n; ++k) {
        int sum = 0;
        for (int t = 1; ; ++t) {
            idx_t g1 = (idx_t)t * ((idx_t)3 * t - 1) / 2;
            if (g1 > k) break;
            int sign = (t & 1) ? 1 : -1;
            sum += sign * part_mod[k - g1];

            idx_t g2 = (idx_t)t * ((idx_t)3 * t + 1) / 2;
            if (g2 > k) continue;
            sum += sign * part_mod[k - g2];
        }
        int mod = sum % 3;
        if (mod < 0) mod += 3;
        part_mod[k] = (uint8_t)mod;
    }
    part_len = n + 1;
}

/* Ensure that the qualifying list contains at least 'target' terms. */
static void extend_qualifying(idx_t target)
{
    while (qual_len < target) {
        ++last_checked;
        extend_partitions(last_checked);

        if (last_checked % 3 == 0 && part_mod[last_checked] == 0) {
            /* Store the qualifying number. */
            if (qual_len == qual_cap) {
                size_t new_cap = qual_cap ? qual_cap * 2 : 8;
                idx_t *tmp = (idx_t *)realloc(qual, new_cap * sizeof(idx_t));
                if (!tmp) exit(1);
                qual = tmp;
                qual_cap = new_cap;
            }
            qual[qual_len++] = last_checked;
        }
    }
}

/* Public entry point required by the OEIS harness.  a(1) returns the
 * first term of the sequence, a(2) the second, etc. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for index 0 – behaviour
                                     below the offset is irrelevant */
    extend_qualifying(n);
    return (seq_t)qual[n - 1];
}
