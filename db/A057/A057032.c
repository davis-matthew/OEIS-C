/*
 * A057032  Let P(n) of a sequence s(1), s(2), s(3), ... be obtained by
 *          leaving s(1), ..., s(n-1) fixed and forward-cyclically permuting
 *          every n consecutive terms thereafter; apply P(2) to 1, 2, 3, ...
 *          to get PS(2), then apply P(3) to PS(2) to get PS(3), then apply
 *          P(4) to PS(3), etc. The limit of PS(n) as n...
 * https://oeis.org/A057032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A057064(n+1) – 1 (computed by applying the P(k)
 *            permutations to the sequence 1,2,3,… up to index n)
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N   20000          /* assumed maximum n needed (≈10 000); adjust if larger values are requested */
#define ARR_LEN (MAX_N * 2)    /* array size sufficient for all blocks that affect positions ≤ MAX_N */

static bool initialized = false;
static idx_t processed_up_to = 0;            /* largest k already processed */
static seq_t arr[ARR_LEN + 1];              /* 1‑based indexing; arr[0] is unused */

/* Helper to ensure the array exists and contains the initial sequence 1,2,3,… */
static void init_array(void)
{
    if (initialized) return;
    for (size_t i = 1; i <= ARR_LEN; ++i)
        arr[i] = (seq_t)i;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0; return 0 for safety */

    init_array();

    /* Apply permutations P(k) for k = processed_up_to+1 … n */
    for (idx_t k = processed_up_to + 1; k <= n; ++k) {
        if (k < 2) continue;   /* P(1) has no effect */

        /* Rotate each block of length k starting at position k */
        for (size_t start = (size_t)k; start + (size_t)k - 1 <= ARR_LEN; start += (size_t)k) {
            seq_t last = arr[start + (size_t)k - 1];
            for (size_t i = start + (size_t)k - 1; i > start; --i)
                arr[i] = arr[i - 1];
            arr[start] = last;
        }
    }

    processed_up_to = n;
    return arr[(size_t)n];
}
