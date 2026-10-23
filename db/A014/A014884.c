/*
 * A014884  n is equal to the number of 1's in all numbers <= n written in
 *          base 9.
 * https://oeis.org/A014884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n‑th integer m such that * the total number of digit
 *            '1' in the base‑9 representations of all * numbers 1..m equals
 *            m. (OEIS A014884)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*------------------------------------------------------------
 * State used to generate the sequence lazily.
 * The cache holds the terms already found.
 * The generator walks through natural numbers, keeping the
 * cumulative difference  d(m) = #(ones in 1..m) – m.
 * Whenever d(m) == 0 we have found a term.
 *------------------------------------------------------------*/
static seq_t *CACHE   = NULL;      /* array of found terms      */
static size_t  CAP    = 0;         /* current capacity         */
static size_t  LEN    = 0;         /* number of terms found    */

static seq_t  CURR    = 0;         /* current number m         */
static long long DIFF = 0;         /* d(m) for CURR            */

/* digits of CURR in base‑9 (little endian), digits[0] least
 * significant digit.  The array is large enough for all 128‑bit
 * numbers. */
static unsigned char DIG[128];
static size_t   D_LEN = 0;         /* number of used digits     */
static int      ONES  = 0;         /* number of '1' digits in CURR */

/*------------------------------------------------------------
 * Increment CURR by one, update DIG array and ONES.
 *------------------------------------------------------------*/
static void inc_and_update(void)
{
    size_t pos = 0;
    while (1) {
        unsigned char prev = DIG[pos];
        unsigned char next = prev + 1;
        if (next == 9) {                 /* carry out */
            DIG[pos] = 0;
            if (prev == 1) ONES -= 1;
            pos++;
            if (pos == D_LEN) {          /* new most‑significant digit */
                D_LEN++;
                DIG[pos] = 0;
            }
            continue;
        } else {                         /* normal increment */
            DIG[pos] = next;
            if (prev == 1) ONES -= 1;
            if (next == 1) ONES += 1;
            break;
        }
    }
    CURR += 1;
}

/*------------------------------------------------------------
 * Generate terms until we have at least 'target' of them.
 * (Indexing from 1: a(1) is the first term.)
 *------------------------------------------------------------*/
static void generate_to(size_t target)
{
    if (LEN >= target) return;         /* already enough terms */

    /* initialise on first use */
    if (LEN == 0) {
        CAP  = 16;
        CACHE = (seq_t *)malloc(CAP * sizeof(seq_t));

        CURR   = 1;
        DIFF   = 0;                    /* count(1)=1, so 1-1=0 */
        D_LEN  = 1;
        DIG[0] = 1;
        ONES   = 1;

        CACHE[LEN++] = CURR;           /* store first term a(1)=1 */
    }

    /* continue generating */
    while (LEN < target) {
        inc_and_update();                /* CURR++, DIG, ONES updated */
        DIFF += (long long)(ONES - 1);   /* update cumulative difference */

        if (DIFF == 0) {                 /* CURR is the next term */
            if (LEN == CAP) {
                CAP = CAP * 2 + 1;
                CACHE = (seq_t *)realloc(CACHE, CAP * sizeof(seq_t));
            }
            CACHE[LEN++] = CURR;
        }
    }
}

/*------------------------------------------------------------
 * Public function required by the task.
 *------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1 */

    generate_to((size_t)n);

    if (n <= LEN) return CACHE[n - 1];
    /* No further terms exist beyond the precomputed ones. */
    return 0;
}
