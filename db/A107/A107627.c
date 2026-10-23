/*
 * A107627  Numbers n such that every digit of n and sqrt(n) contains a loop
 *          (only digits 0,4,6,8,9 in n and sqrt(n)).
 * https://oeis.org/A107627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, dumb
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   has_loop_digits_uint128, root_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that every digit of n and √n contains a loop
   (digits 0,4,6,8,9).  The terms are the squares of numbers composed only of
   those digits, and the squares themselves must also have only those digits.
   Generation is done breadth‑first on the roots, keeping the list of found
   terms in ascending order.                                                     */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_loop_digits_uint128.h"
#include "../utils/root_square.h"

/* ---------- Dynamic storage for terms and the root queue ----------------- */
static seq_t *term_arr = NULL;   /* array of found terms (squares)   */
static idx_t term_cap = 0, term_cnt = 0;

static seq_t *queue_arr = NULL;   /* queue of roots to process        */
static idx_t queue_cap = 0;
static idx_t queue_pos = 0;       /* next element to pop              */
static idx_t queue_end = 0;       /* one past last element            */

/* Ensure capacity for the term array, doubling as needed.                 */
static void ensure_term_cap(idx_t needed)
{
    if (needed <= term_cap) return;
    idx_t new_cap = term_cap ? term_cap << 1 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(term_arr, new_cap * sizeof(seq_t));
    term_arr = new_arr;
    term_cap = new_cap;
}

/* Ensure capacity for the queue array.                                     */
static void ensure_queue_cap(idx_t needed)
{
    if (needed <= queue_cap) return;
    idx_t new_cap = queue_cap ? queue_cap << 1 : 256;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(queue_arr, new_cap * sizeof(seq_t));
    queue_arr = new_arr;
    queue_cap = new_cap;
}

/* Push a new root into the queue.                                          */
static void queue_push(seq_t v)
{
    ensure_queue_cap(queue_end + 1);
    queue_arr[queue_end++] = v;
}

/* Initialise the queue with the single‑digit loop numbers.                 */
static void init_queue(void)
{
    if (queue_arr) return;   /* already initialised */
    queue_arr = NULL;
    queue_cap = 0;
    queue_pos = queue_end = 0;
    queue_push(4);
    queue_push(6);
    queue_push(8);
    queue_push(9);
}

/* Generate terms until we have at least n of them.                         */
static void generate_until(idx_t n)
{
    init_queue();
    while (term_cnt < n) {
        if (queue_pos >= queue_end) break;   /* safety, should not happen */

        seq_t root = queue_arr[queue_pos++];
        seq_t sq   = root_square(root);

        if (sq && has_loop_digits_uint128((unsigned __int128)sq)) {
            ensure_term_cap(term_cnt + 1);
            term_arr[term_cnt++] = sq;
        }

        /* Generate children: append each loop digit to the root. */
        const unsigned char digits[5] = {0, 4, 6, 8, 9};
        for (int i = 0; i < 5; ++i) {
            unsigned d = digits[i];
            unsigned __int128 child128 = (unsigned __int128)(seq_t)root * 10 + d;
            seq_t max_val = (seq_t)-1;
            if (child128 > (unsigned __int128)max_val) continue; /* overflow for seq_t */
            queue_push((seq_t)child128);
        }
    }
}

/* ---------- External function required by the test harness. ------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, guard against underflow */
    if (term_cnt < n) generate_until(n);
    return term_arr[n - 1];
}
