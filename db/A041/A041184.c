/*
 * A041184  Numerators of continued fraction convergents to sqrt(103).
 * https://oeis.org/A041184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of the n-th convergent to sqrt(103) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static const uint32_t D   = 103;   /* D = 103 */
static const uint32_t A0  = 10;    /* floor(sqrt(D)) */

static idx_t          max_n   = 0;   /* highest index computed */
static seq_t        *nums    = NULL; /* array of numerators */
static size_t        num_cap = 0;    /* allocated capacity */
static uint64_t       cur_m   = 0;   /* current m  in the CF algorithm */
static uint64_t       cur_d   = 1;   /* current d  in the CF algorithm */
static uint64_t       cur_a   = A0;  /* current a  in the CF algorithm */
static bool           init    = false;

/* Ensure the array can hold at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (need < num_cap) return;
    size_t new_cap = num_cap ? num_cap : 16;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(nums, new_cap * sizeof(seq_t));
    nums = tmp;
    num_cap = new_cap;
}

/* Compute numerators up to a given index */
static void compute_until(idx_t target)
{
    if (!init) {
        /* initialise with the first term */
        ensure_capacity((size_t)target + 1);
        nums[0] = (seq_t)A0;
        max_n   = 0;
        init    = true;
    }
    if (target <= max_n) return;

    for (uint64_t k = max_n + 1; k <= (uint64_t)target; ++k) {
        uint64_t m_next = cur_d * cur_a - cur_m;
        uint64_t d_next = (D - m_next * m_next) / cur_d;
        uint64_t a_next = (A0 + m_next) / d_next; /* floor division */

        seq_t numer;
        if (k == 1) {
            /* P1 = a1 * P0 + P_{-1} (P_{-1}=1) */
            numer = (seq_t)a_next * nums[0] + (seq_t)1;
        } else {
            numer = (seq_t)a_next * nums[k - 1] + nums[k - 2];
        }

        ensure_capacity((size_t)k);
        nums[k] = numer;

        /* update state for next coefficient */
        cur_m = m_next;
        cur_d = d_next;
        cur_a = a_next;
    }
    max_n = target;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    compute_until(n);
    return nums[n];
}
