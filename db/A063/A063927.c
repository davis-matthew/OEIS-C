/*
 * A063927  Number of vertices in the regular 4-dimensional polytopes.
 * https://oeis.org/A063927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The regular 4‑dimensional convex polytopes are
   1:  5‑cell          – simplex          V = d+1 = 5
   2:  8‑cell          – hypercube        V = 2^d = 16
   3: 16‑cell          – cross–polytope   V = 2·d = 8
   4: 24‑cell          – self‑dual 24‑cell V = 24
   5: 600‑cell         – 600‑cell         V = 600
   6: 120‑cell         – 120‑cell         V = 120
   Here d = 4 is the dimension of each polytope.
   For any other n the sequence is undefined, so return 0.  */
seq_t a(idx_t n)
{
    switch ((uint64_t)n) {
    case 1:  /* 5‑cell (simplex) */
        return (seq_t)5;
    case 2:  /* 8‑cell (hypercube) */
        return (seq_t)16;
    case 3:  /* 16‑cell (cross‑polytope) */
        return (seq_t)8;
    case 4:  /* 24‑cell */
        return (seq_t)24;
    case 5:  /* 600‑cell */
        return (seq_t)600;
    case 6:  /* 120‑cell */
        return (seq_t)120;
    default: /* Out of known range */
        return (seq_t)0;
    }
}
