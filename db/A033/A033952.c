/*
 * A033952  Write 1,2,... in a clockwise spiral on a square lattice, writing
 *          each digit at a separate lattice point, starting with 1 at the
 *          origin and 2 at x=0, y=-1; sequence gives the numbers on the
 *          positive x-axis.
 * https://oeis.org/A033952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   Compute the digit that lies on the positive x‑axis (including
 *            the origin) of the clockwise Champernowne spiral. The digit at
 *            position (k,0) is a(k+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_count.h"
 
/* External linker symbol types */
 
/* seq_t and idx_t are defined by the build system. */
 
/* --------------------------------------------------------------------------- */
/*  Spiral state: kept statically so that callers may request values in any order.  */
/* --------------------------------------------------------------------------- */
 
typedef struct {
    int  x;                      /* current x coordinate */
    int  y;                      /* current y coordinate */
    int  dir;                    /* 0 south, 1 west, 2 north, 3 east */
    idx_t  step_len;             /* current leg length */
    idx_t  steps_done;           /* steps taken in current leg */
    idx_t  leg_count;            /* number of legs completed (used to grow step_len) */
    idx_t  number;               /* current decimal number being written */
    unsigned char digits[50];    /* digits of current number (most significant first) */
    int  dpos;                   /* index of next digit to write (0 … dlen-1) */
    int  dlen;                   /* number of digits in current number */
} SpiralState;
 
static SpiralState sp = {0};                /* static state instance (zero‑initialized) */
static bool state_inited = false;           /* indicates whether sp is initialized */
 
/* --------------------------------------------------------------------------- */
/*  Axial cache: digits of the positive x–axis (including origin).             */
/* --------------------------------------------------------------------------- */
 
static uint8_t *axis_digits      = NULL;   /* array of digits (0‑9) */
static idx_t    axis_cap         = 0;      /* allocated capacity */
static idx_t    axis_count       = 0;      /* number of digits currently stored   */
 
/* enlarge the axial cache to accommodate at least `min_cap` entries */
static void ensure_axis_capacity(idx_t min_cap) {
    if (min_cap <= axis_cap) return;
    idx_t new_cap = (axis_cap > 0) ? (axis_cap * 2) : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    uint8_t *new_buf = (uint8_t *)realloc(axis_digits, new_cap * sizeof(uint8_t));
    if (!new_buf) exit(EXIT_FAILURE);
    axis_digits = new_buf;
    axis_cap    = new_cap;
}
 
/* compute the digit array for a given number */
static void init_digits(idx_t num) {
    sp.dlen = (num == 0) ? 1 : (int)digit_count(num);
    idx_t tmp = num;
    for (int i = sp.dlen - 1; i >= 0; --i) {
        sp.digits[i] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    sp.dpos = 0;
}
 
/* place a digit on the positive x‑axis (if applicable) */
static void record_current_position(void) {
    if (sp.y == 0 && sp.x >= 0) {
        ensure_axis_capacity(axis_count + 1);
        axis_digits[axis_count] = sp.digits[sp.dpos];
        ++axis_count;
    }
}
 
/* advance the spiral one step (writing the next digit) */
static void advance_one_step(void) {
    /* move to next lattice point */
    switch (sp.dir) {
        case 0: --sp.y; break;          /* south  */
        case 1: --sp.x; break;          /* west   */
        case 2: ++sp.y; break;          /* north  */
        case 3: ++sp.x; break;          /* east   */
    }
    ++sp.steps_done;
 
    /* record if on the positive x‑axis */
    record_current_position();
 
    /* advance the digit pointer */
    if (sp.dpos + 1 < sp.dlen) {
        ++sp.dpos;
    } else {                            /* finished current number */
        ++sp.number;
        init_digits(sp.number);
    }
 
    /* finish leg if needed */
    if (sp.steps_done == sp.step_len) {
        sp.steps_done = 0;
        sp.dir = (sp.dir + 1) % 4;
        ++sp.leg_count;
        if (sp.leg_count % 2 == 0) ++sp.step_len;
    }
}
 
/* generate axial digits up to (but including) index `max_index` (0‑based) */
static void generate_axis_to(idx_t max_index) {
    if (!state_inited) {
        /* initialise the state */
        sp.x = 0; sp.y = 0; sp.dir = 0; sp.step_len = 1;
        sp.steps_done = 0; sp.leg_count = 0; sp.number = 1;
        init_digits(sp.number);
        state_inited = true;
        /* origin: handle the first position */
        record_current_position();
        /* advance digit pointer after placing the first digit */
        if (sp.dpos + 1 < sp.dlen) {
            ++sp.dpos;
        } else {
            ++sp.number;
            init_digits(sp.number);
        }
    }
 
    while (axis_count <= max_index) {
        advance_one_step();
    }
}
 
/* --------------------------------------------------------------------------- */
/*  Public sequence function                                          */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined (offset is 1), but return 0 */
    /* indices for cache are 0‑based */
    idx_t target_index = n - 1;
    if (axis_count <= target_index) {
        generate_axis_to(target_index);
    }
    return (seq_t)axis_digits[target_index];
}
