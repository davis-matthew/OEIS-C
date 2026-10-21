/*
 * A080197  13-smooth numbers: numbers whose prime divisors are all <= 13.
 * https://oeis.org/A080197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 1574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth 13‑smooth number (numbers composed of primes ≤ 13)
   using the classic linear Hamming‑number algorithm. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *smooth = NULL;    /* array of generated numbers */
static size_t smooth_cap = 0;   /* allocated capacity */
static size_t smooth_len = 0;   /* number of numbers stored */
static idx_t indices[6] = {0, 0, 0, 0, 0, 0};
static const int primes[6] = {2, 3, 5, 7, 11, 13};

/* ensure at least n elements are generated */
static void ensure_smooth(idx_t n)
{
    if (n <= smooth_len) return;          /* nothing to do */

    /* initialise storage if needed */
    if (smooth == NULL) {
        smooth_cap = 64;
        smooth = (seq_t*)malloc(smooth_cap * sizeof(seq_t));
        smooth[0] = (seq_t)1;         /* a(1) = 1 */
        smooth_len = 1;
        for (int i = 0; i < 6; ++i) indices[i] = 0;
    }

    /* allocate more space if necessary */
    while (smooth_len < n) {
        if (smooth_len == smooth_cap) {
            smooth_cap = (smooth_cap == 0) ? 64 : smooth_cap * 2;
            smooth = (seq_t*)realloc(smooth, smooth_cap * sizeof(seq_t));
        }

        seq_t candidates[6];
        for (int i = 0; i < 6; ++i) {
            candidates[i] = smooth[indices[i]] * primes[i];
        }

        /* find minimum candidate */
        seq_t next = candidates[0];
        for (int i = 1; i < 6; ++i) {
            if (candidates[i] < next) next = candidates[i];
        }

        smooth[smooth_len++] = next;

        /* bump all indices that produced the minimum */
        for (int i = 0; i < 6; ++i) {
            if (candidates[i] == next) ++indices[i];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 */
    ensure_smooth(n);
    return smooth[n - 1];
}
