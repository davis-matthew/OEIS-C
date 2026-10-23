/*
 * A159305  Sequence of happy tribonacci numbers.
 * https://oeis.org/A159305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth Tribonacci number that is also a happy number.
 *            The tribonacci sequence is defined by T(0)=0, T(1)=0, T(2)=1,
 *            T(k)=T(k‑1)+T(k‑2)+T(k‑3) (k≥3). a(n) is the n‑th element of the
 *            intersection of A000073 and A007770. Offset: 1 (i.e. a(1) = 1)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 17,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"

/* Test whether a seq_t value is a happy number.  */
static bool is_happy(seq_t n)
{
    if (n == (seq_t)1)
        return true;
    while (n != (seq_t)1 && n != (seq_t)4)
    {
        seq_t sum = 0;
        seq_t m = n;
        while (m > 0)
        {
            seq_t d = m % 10;
            sum += d * d;
            m /= 10;
        }
        n = sum;
    }
    return n == (seq_t)1;
}

/* Return the n‑th Tribonacci number that is also a happy number.  */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n=0 – guard anyway */
        return 0;

    /* Static state for generation and memoisation.  */
    static bool init = false;
    static seq_t trib_a = (seq_t)0;      /* T(k‑2)   */
    static seq_t trib_b = (seq_t)0;      /* T(k‑1)   */
    static seq_t trib_c = (seq_t)1;      /* T(k)     */
    static idx_t trib_index = 2;         /* current index of trib_c (0‑based) */

    static seq_t *happy_cache = NULL;
    static size_t happy_cap   = 0;
    static size_t happy_count = 0;
    static seq_t last_added   = (seq_t)-1;  /* sentinel for duplicates */

    if (!init)
    {
        /* Cache the first happy tribonacci number, T(2)=1.  */
        if (is_happy(trib_c))
        {
            grow_seq(&happy_cache, &happy_cap, 1);
            happy_cache[0] = trib_c;
            happy_count = 1;
            last_added = trib_c;
        }
        init = true;
    }

    if (n <= happy_count)
        return happy_cache[(size_t)(n - 1)];

    /* Generate tribonacci numbers until we have at least n happy ones.  */
    while (happy_count < (size_t)n)
    {
        seq_t next = trib_a + trib_b + trib_c;
        trib_a = trib_b;
        trib_b = trib_c;
        trib_c = next;
        ++trib_index;

        if (is_happy(trib_c))
        {
            if (trib_c == last_added)     /* skip duplicate values */
                continue;

            if (happy_count == happy_cap)
                grow_seq(&happy_cache, &happy_cap, happy_count + 1);
            happy_cache[happy_count++] = trib_c;
            last_added = trib_c;
        }
    }

    return happy_cache[(size_t)(n - 1)];
}
