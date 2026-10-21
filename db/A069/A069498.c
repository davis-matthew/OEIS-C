/*
 * A069498  Triangular numbers of the form 10*k.
 * https://oeis.org/A069498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): the n‑th triangular number that is a multiple of 10.
   The sequence starts with a(1)=0, a(2)=10, a(3)=120, …  */
/* We do not have a closed formula for the index, so we scan
   successive integers t, compute T(t)=t*(t+1)/2, and count
   those that are ≡0 (mod 10).  */
seq_t a(idx_t n) {
    idx_t count = 0;   /* how many qualifying terms we have seen */
    for (idx_t t = 0;; ++t) {
        seq_t val = (seq_t)t * ((seq_t)t + 1) / 2;
        if ((val % (seq_t)10) == 0) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
