/*
 * A096063  Beginning with the sequence S(1)={1} form, in succession, the
 *          sequence S(n) for n=2,3,4,... by inserting the n smallest
 *          multiples of n that have not been used previously, in order of
 *          increasing size, n-1 of them between the final n terms of S(n-1)
 *          and the final multiple after the last term of S(n...
 * https://oeis.org/A096063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 8001, u32 n <= 8001, u64 n <= 8001,
 *            u128 n <= 8001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construction: iterative insertion described in OEIS entry A096063 */
/* The function returns the nth term of the sequence defined by  
   repeatedly inserting the smallest unused multiples of n into the
   existing sequence.  It terminates as soon as the value at index n
   becomes immutable.  */

/*  Include the standard headers needed by both C and C++ builds.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*  Helper types supplied by the build environment.  */
/*  seq_t and idx_t are defined externally; we only use them.  */

/*  Dynamically allocated bitmap of numbers that have already appeared.
    We store one byte per number; only the indices used are
    guaranteed to be < used_cap.  */
static bool *used = NULL;
static size_t used_cap = 0;

/*  Ensure the bitmap can address numbers up to m (inclusive).  */
static void ensure_used_capacity(idx_t m)
{
    if ((idx_t)used_cap > m) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while ((idx_t)new_cap <= m) new_cap <<= 1;
    void *tmp = realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    used = (bool*)tmp;
    memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used_cap = new_cap;
}

/*  Mark a number as used.  */
static inline void set_used(idx_t m)
{
    ensure_used_capacity(m);
    used[m] = true;
}

/*  Query whether a number has been used.  */
static inline bool is_used(idx_t m)
{
    return m < (idx_t)used_cap && used[m];
}

/*  Sequence storage.  The array grows as more terms are generated.  */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;    /* current number of terms stored   */
static size_t seq_cap = 0;    /* allocated capacity of seq_arr    */
static idx_t cur_step = 1;    /* last step that has been processed */

/*  Ensure the sequence array has room for at least 'need' elements.  */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    void *tmp = realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_arr = (seq_t*)tmp;
    seq_cap = new_cap;
}

/*  Initialise everything on the first call.  */
static void init_once(void)
{
    if (seq_arr) return;         /* already initialised */
    ensure_seq_capacity(8);
    seq_arr[0] = (seq_t)1;       /* S(1) = {1}           */
    seq_len  = 1;
    set_used(1);
    cur_step = 1;
}

/*  Return the nth term of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    init_once();

    /*  Continuously perform steps until the position n has stabilised.
        The value at position n is immutable when the last processed
        step is at least (seq_len - n).  */
    for (;;) {

        /*  Do we already have a stable value at index n?  */
        if (n <= (idx_t)(seq_len - cur_step))
            return seq_arr[(size_t)n - 1];

        /*  Next step: insert the next set of multiples.  */
        ++cur_step;                       /* step number = cur_step           */
        idx_t k = cur_step;                /* number of multiples to insert    */
        idx_t needed = seq_len + k;        /* new length after insertion       */

        /*  Prepare array to collect the k smallest unused multiples.  */
        static seq_t mul_buf[20005];       /* safe for n <= 10000              */
        idx_t cnt = 0;
        idx_t mult = k;                   /* first multiple of k */
        while (cnt < k) {
            if (!is_used(mult)) {
                mul_buf[cnt++] = (seq_t)mult;
                set_used(mult);
            }
            mult += k;
        }

        /*  Special handling for k == 2 to match the initial example.  */
        if (k == 2) {
            ensure_seq_capacity(3);
            seq_arr[0] = mul_buf[0];      /* 2 */
            seq_arr[1] = (seq_t)1;        /* original 1 */
            seq_arr[2] = mul_buf[1];      /* 4 */
            seq_len = 3;
            continue;
        }

        /*  For k >= 3: insert the multiples between the last k terms.  */
        size_t prefix_len = seq_len - k;           /* terms before the tail */
        seq_len = needed;                           /* new length          */
        ensure_seq_capacity(seq_len);

        /*  Copy the old tail terms into a temporary buffer.  */
        static seq_t old_tail[20005];
        for (size_t i = 0; i < k; ++i)
            old_tail[i] = seq_arr[prefix_len + i];

        /*  Rebuild the sequence: keep prefix, then interleave.  */
        size_t idx = prefix_len;                     /* write position */
        for (size_t i = 0; i < k; ++i) {
            seq_arr[idx++] = old_tail[i];
            if (i < k - 1)                       /* insert intermediate multiple */
                seq_arr[idx++] = mul_buf[i];
        }
        seq_arr[idx++] = mul_buf[k - 1];            /* final multiple at end */

        /*  Loop to check stabilisation condition again.  */
    }
}
