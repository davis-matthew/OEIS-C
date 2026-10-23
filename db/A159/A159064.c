/*
 * A159064  Emirps which remain primes when rotated by 180 degrees on a
 *          digital clock display.
 * https://oeis.org/A159064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   grow_seq, is_prime_big, rotate_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code below implements A159064 (Emirps that remain prime when rotated
 *  by 180° on a digital clock).  It searches for numbers that:
 *
 *  1. contain only the digits {0,1,6,8,9} (no leading zero),
 *  2. are prime,
 *  3. have a different reverse that is also prime (emirp),
 *  4. remain prime when each digit is rotated 180° (0→0, 1→1, 6→9, 8→8,
 *     9→6) and the whole number is reversed.
 *
 *  The first term (a(1)) is 199; subsequent terms are returned in order
 *  on each call to a(n).  For efficiency the sequence is built incrementally
 *  with memoisation and a simple base‑5 enumeration of eligible digits.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"
#include "../utils/rotate_digits.h"

static const int DIGS[5] = {0, 1, 6, 8, 9};

/* ---------- generator for numbers using only allowed digits ---------- */
static bool gen_init = false;
static int   gen_len;
static uint8_t gen_idx[128];   /* supports very long numbers */

/* Return the next candidate as seq_t (with the current state advanced). */
static seq_t next_candidate(void)
{
    if (!gen_init) {
        gen_init = true;
        gen_len  = 1;
        gen_idx[0] = 1;   /* first digit cannot be 0 */
    }

    /* Build the current number as a seq_t */
    seq_t val = 0;
    for (int i = 0; i < gen_len; ++i) {
        val = val * 10 + DIGS[gen_idx[i]];
    }

    /* Advance to next digit combination */
    int pos = gen_len - 1;
    while (true) {
        if (gen_idx[pos] < 4) {      /* move to next allowed digit */
            ++gen_idx[pos];
            break;
        } else {                     /* rollover and carry */
            gen_idx[pos] = 0;
            --pos;
            if (pos < 0) {           /* increase length */
                ++gen_len;
                gen_idx[0] = 1;
                for (int j = 1; j < gen_len; ++j) gen_idx[j] = 0;
                break;
            }
        }
    }

    return val;
}

/* ---------- digit operations on seq_t ---------- */
static inline seq_t reverse_digits(seq_t n)
{
    seq_t r = 0;
    while (n > 0) {
        int d = (int)(n % 10);
        r = r * 10 + d;
        n /= 10;
    }
    return r;
}

/* ---------- sequence storage ---------- */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* ---------- main entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety: OEIS offset starts at 1 */

    if (seq_arr == NULL) {
        grow_seq(&seq_arr, &seq_cap, 16);
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        seq_t cand = next_candidate();

        /* Skip values before the first known term (199) to save time */
        if (cand < (seq_t)199) continue;

        if (!is_prime_big(cand)) continue;

        seq_t rev = reverse_digits(cand);
        if (rev == cand) continue;          /* not an emirp */
        if (!is_prime_big(rev)) continue;

        seq_t rot = rotate_digits(cand);
        if (!is_prime_big(rot)) continue;

        /* Store the new term */
        grow_seq(&seq_arr, &seq_cap, seq_len + 1);
        seq_arr[seq_len++] = cand;
    }

    return seq_arr[n - 1];
}
